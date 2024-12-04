/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 16:34:43 by bclaeys           #+#    #+#             */
/*   Updated: 2024/12/04 17:51:56 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

int	restore_fds(t_var_data *var_data)
{
	if (dup2(var_data->std_input_fd_backup, STDIN_FILENO) == -1) 
		return (ft_printf("Error: dup2 failed\n"), 1);
	if (dup2(var_data->std_output_fd_backup, STDOUT_FILENO) == -1)
		return (ft_printf("Error: dup2 failed\n"), 1);
	return (var_data->pipe_check = false, 0);
}

int	check_if_pipe(t_var_data *var_data, t_ast_node *curr_node_pipe)
{
	int	pipe_fd[2];

	if (!curr_node_pipe)	
	{
		if (var_data->pipe_check)
			if (dup2(var_data->std_output_fd_backup, STDOUT_FILENO) == -1)
				return (ft_printf("Error: dup2 failed\n"), 1);
		return (0);
	}
	if (pipe(pipe_fd) == -1)
		return (1);
	if (dup2(pipe_fd[0], STDIN_FILENO) == -1) 
		return (ft_printf("Error: dup2 failed\n"), 1);
	if (dup2(pipe_fd[1], STDOUT_FILENO) == -1) 
		return (ft_printf("Error: dup2 failed\n"), 1);
	var_data->pipe_check = true;
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	return (0);
}
