/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_main.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 11:48:11 by bclaeys           #+#    #+#             */
/*   Updated: 2025/01/07 14:00:47 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	main(int ac, char **av, char **envp)
{
	t_var_data	*var_data;

	(void)av;
	if (ac != 1)
		return (1);
	var_data = init_var_data(envp);
	if (!var_data)
		return (1);
	sighandler(var_data, MAIN_PROCESS);
	if (ms_command_line_inteface(var_data))
		return (free_var_data(var_data), 1);
	restore_tty(var_data);
	free_var_data(var_data);
	return (0);
}
/* TESTEN */
/* sowieso alles in valgrind testen op uninit values enzovoort */
/* errors naar STDERRROR */
