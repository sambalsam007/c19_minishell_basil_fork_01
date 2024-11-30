/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_main.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 11:48:11 by bclaeys           #+#    #+#             */
/*   Updated: 2024/11/28 13:11:30 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	main(int ac, char **av, char **envp)
{
	t_var_data	*var_data;

	(void)av;
	if (ac != 1)
		return (1);
	/* system("valgrind ./minishell"); */
	var_data = init_var_data(envp);
	if (!var_data)
		return (1);
	// sighandler
	if (ms_command_line_inteface(var_data))
		return (free_var_data(var_data), 1);
	free_var_data(var_data);
	return (0);
}
