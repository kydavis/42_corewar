/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mburson <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/04 18:52:02 by mburson           #+#    #+#             */
/*   Updated: 2017/03/04 18:52:04 by mburson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <corewar.h>

/*
** changes endianess of an unsigned in
** champs are compiled with big endian and this runs in little endian
*/

static unsigned int		fix_end(unsigned int n)
{
	return (
		((n >> 24) & 0xFF)
		| ((n >> 8) & 0xFF00)
		| ((n << 8) & 0xFF0000)
		| (n << 24));
}

static int				load_champion(char const *file,
							void *loc, struct s_champ *champ)
{
	t_header	header;
	int			fd;
	ssize_t		size;

	if (-1 == (fd = open(file, O_RDONLY))
		|| -1 == (size = read(fd, &header, sizeof(header))))
		return (-1);
	if (size != sizeof(header))
	{
		g_error = 2;
		return (-1);
	}
	header.prog_size = fix_end(header.prog_size);
	if (header.prog_size > CHAMP_MAX_SIZE
		|| -1 == (size = read(fd, loc, header.prog_size + 1))
		|| size != header.prog_size)
	{
		g_error = 2;
		return (-1);
	}
	if (-1 == close(fd))
		return (-1);
	ft_memmove(champ->prog_name, header.prog_name, sizeof(header.prog_name));
	ft_memmove(champ->comment, header.comment, sizeof(header.comment));
	return (size);
}

/*
** process.coundown is decremented at the start of each step
** so it needs to start out at 1
**
** process are added to the front of the linked list so that the last process
** added becomes the first process
*/

static int				add_process(t_list **processes, uint8_t *pc,
							int champ_index)
{
	struct s_process	*p;
	t_list				*link;

	if (NULL == (p = (struct s_process*)malloc(sizeof(*p))))
		return (-1);
	ft_bzero(p, sizeof(*p));
	p->registors[0] = ~(t_reg)champ_index;
	change_end(&p->registors[0], sizeof(p->registors[0]));
	p->pc = pc;
	p->op_code = *pc;
	p->countdown = g_op_tab[p->op_code].cycles_required;
	p->champ_index = champ_index;
	p->pid = g_flags.pid++;
	if (NULL == (link = lstnew((void*)p)))
		return (-1);
	lstadd(processes, link);
	return (0);
}

static int				add_champs_processes(char **champs, struct s_game *game)
{
	size_t		offset;
	uint8_t		*start_loc;
	int			champ_index;
	int			champ_size;
	int			i;

	offset = MEM_SIZE / game->champ_count;
	start_loc = game->arena;
	i = 0;
	champ_index = 0;
	while (i < MAX_PLAYERS)
	{
		if (champs[i])
		{
			if (-1 == (champ_size = load_champion(champs[i], start_loc,
					game->champs + champ_index))
				|| -1 == add_process(&game->processes, start_loc, champ_index))
				return (-1);
			game->last_live_champ = game->champs + champ_index;
			arena_writer(game, start_loc, champ_size, ++champ_index);
		}
		i++;
		start_loc += offset;
	}
	return (0);
}

/*
** initalizes the game struct and take as arguments a list of champ files and a
** pointer to the game that is being initized
*/

int						init_game_struct(char **champs, struct s_game *game)
{
	int			champ_count;
	int			i;

	champ_count = 0;
	i = 0;
	ft_bzero(game, sizeof(*game));
	while (i < MAX_PLAYERS)
	{
		if (champs[i])
			champ_count++;
		i++;
	}
	if (champ_count == 0)
	{
		g_error = 4;
		return (-1);
	}
	g_flags.pid = 0;
	game->champ_count = champ_count;
	game->cycles_to_death = CYCLE_TO_DIE;
	game->end_state = NOT_OVER;
	if (-1 == add_champs_processes(champs, game))
		return (-1);
	return (0);
}
