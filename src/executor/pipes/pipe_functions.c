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
#include <unistd.h>

int	restore_fds(t_var_data *var_data)
{
	if (dup2(var_data->std_input_fd_backup, STDIN_FILENO) == -1) 
		return (ft_printf("Error: dup2 failed\n"), 1);
	if (dup2(var_data->std_output_fd_backup, STDOUT_FILENO) == -1)
		return (ft_printf("Error: dup2 failed\n"), 1);
	return (var_data->pipe_check = false, 0);
}

int	check_pipe(t_var_data *var_data, t_ast_node *curr_node_pipe)
{
		if (curr_node_pipe == var_data->first_node_ast && curr_node_pipe->pipe) 
		{
			write (var_data->std_output_fd_backup, "test1\n", 6);//TEST
			if (dup2(var_data->pipe_fd[1], STDOUT_FILENO) == -1) 
				return (ft_printf("Error: dup2 failed\n"), 1);
			close(var_data->pipe_fd[0]);
		}
		else if (curr_node_pipe != var_data->first_node_ast)
		{
			write (var_data->std_output_fd_backup, "test2\n", 6);//TEST
			if (dup2(var_data->pipe_fd[0], STDIN_FILENO) == -1) 
				return (ft_printf("Error: dup2 failed\n"), 1);
			if (curr_node_pipe->pipe)
			{
				if (dup2(var_data->pipe_fd[1], STDOUT_FILENO) == -1) 
					return (ft_printf("Error: dup2 failed\n"), 1);
			}
			else 
			{
				write(var_data->std_output_fd_backup, "test3\n", 6);//TEST
				close(var_data->pipe_fd[1]);
			}
			var_data->pipe_check = true;
		}
	return (0);
	/* Alles hieronder was tot voor kort de check_if_pipe, die aangeroepen werd in execute_logic  */
	/* if (curr_node_pipe != var_data->first_node_ast && var_data->pipe_check)  */
	/* { */
	/* 	ft_printf("test84384\n"); */
	/* 	dup2(var_data->pipe_fd[0], STDIN_FILENO); */
	/* } */
	/* if (!curr_node_pipe) */
	/* { */
		/* if (var_data->pipe_check) */
		/* { */
		/* 	close(var_data->pipe_fd[1]);	 */
		/* 	if (dup2(var_data->std_output_fd_backup, STDOUT_FILENO) == -1) */
		/* 		return (1); */
		/* } */
	/* 	return (0); */
	/* } */
	/* if (pipe(var_data->pipe_fd) == -1) */
	/* 	return (1); */
	/* if (dup2(var_data->pipe_fd[0], STDIN_FILENO) == -1)  */
	/* 	return (ft_printf("Error: dup2 failed\n"), 1); */
	/* if (dup2(var_data->pipe_fd[1], STDOUT_FILENO) == -1)  */
	/* 	return (ft_printf("Error: dup2 failed\n"), 1); */
	/* close(STDIN_FILENO); */
	/* close(STDOUT_FILENO); */
	/* var_data->pipe_check = true; */
	/* close(var_data->pipe_fd[0]); */
	/* close(var_data->pipe_fd[1]); */
	/* return (0); */
}
