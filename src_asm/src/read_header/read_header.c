/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_header.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdavis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/17 21:03:07 by kdavis            #+#    #+#             */
/*   Updated: 2017/04/19 19:36:29 by kdavis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <asm.h>

/*int	read_name(t_asm *master)
{
	size_t	name_len;

	name_len = ft_strlen(NAME_CMD_STRING);
	if ((next_token(&master->cp) == -1) ||
			ft_strncmp(master->cp, NAME_CMD_STRING, name_len))
		return (7);
	master->cp += name_len;
	if (next_token(&master->cp))
		return (7);
	ft_printf("%.5s\n", master->cp);
	return (0);
}

int	read_header(t_asm *master)
{
	int	ern;
	size_t	len;

	master->header.magic = flip_uint32(COREWAR_EXEC_MAGIC);
	if ((ern = read_name(master)))
		return (ern);
	end = ft_memccpy(m->header.prog_name, m->file.arr, '"', PROG_NAME_LENGTH);
	if (end)
		*(((char*)end) - 1) = 0;
	read_name(header->prog_name, fd);
	read_comment(header->comment, fd);
	return (0);
}*/

char	*skip_whitespace(char *line)
{
	if (line)
		while (ft_iswhitespace(*line))
			line++;
	return (line);
}


int	read_header(char *file, t_asm *master)
{
	char	*line;
	char	*cp;
	int		ern;

	if ((master->fd = open(name, O_RDONLY)) == -1)
		return (print_error(2, NULL, 0, 0));
	while ((ern = get_next_line(master->fd, &line)) > 0)
	{
		cp = skip_whitespace(line);
		if (*cp == COMMAND_CHAR)
			if ((ern = read_command(&master->header, line)))
		if (*cp == COMMENT_CHAR && *cp == '\0')
		//parse line
		ft_strdel(&line);
	}
	if (ern == -1)
		return (print_error(1, NULL, 0, 0));
}