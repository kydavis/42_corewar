
#include <corewar.h>

static t_op_arg	calc_ldi_offset(struct s_process *process,
				struct s_parameter *params)
{
	union u_val	first;
	union u_val	second;

	if (params[0].param_type == T_REG)
		reverse_bytes((void *)&process->registors[params[0].param_val.val], 
			REG_SIZE, first.arr);
	else
		reverse_bytes(params[0].param_val.arr, IND_SIZE, first.arr);
	if (params[1].param_type == T_REG)
		reverse_bytes((void *)&process->registors[params[1].param_val.val], 
			REG_SIZE, second.arr);
	else
		reverse_bytes(params[1].param_val.arr, IND_SIZE, second.arr);
	return (first.val + second.val);
}

/*
**	> Is the carry only modified if the function runs?
*/
int		ldi(struct s_game *game, struct s_process *process)
{
	struct s_parameter	params[g_op_tab[0x0a].argc];
	uint8_t				*pc_temp;
	t_op_arg			offset;

	pc_temp = process->pc;
	if (-1 == parse_and_validate_parameters(game, process, &pc_temp, params))
		return (-1);
	modify_carry(process);
	if (-1 == check_registors(process->op_code, params))
	{
		process->pc = pc_temp;
		return (0);
	}
	offset = calc_ldi_offset(process, params);
	read_arena(game->arena, process->pc + (offset % IDX_MOD),
		(void *)&process->registors[params[2].param_val.val], REG_SIZE);
	process->pc = pc_temp;
	return (0);
}

int		lldi(struct s_game *game, struct s_process *process)
{
	struct s_parameter	params[g_op_tab[0x0e].argc];
	uint8_t				*pc_temp;
	t_op_arg		offset;

	pc_temp = process->pc;
	if (-1 == parse_and_validate_parameters(game, process, &pc_temp, params))
		return (-1);
	if (-1 == check_registors(process->op_code, params))
	{
		process->pc = pc_temp;
		return (0);
	}
	modify_carry(process);
	if (-1 == check_registors(process->op_code, params))
		return (0);
	offset = calc_ldi_offset(process, params);
	read_arena(game->arena, process->pc + offset,
		(void *)&process->registors[params[2].param_val.val], REG_SIZE);
	process->pc = pc_temp;
	return (0);
}