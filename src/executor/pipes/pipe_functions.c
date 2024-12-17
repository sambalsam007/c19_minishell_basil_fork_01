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
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

int	restore_fds(t_var_data *var_data)
{
	if (dup2(var_data->std_input_fd_backup, STDIN_FILENO) == -1) 
		return (ft_printf("Error: dup2 failed\n"), 1);
	if (dup2(var_data->std_output_fd_backup, STDOUT_FILENO) == -1)
		return (ft_printf("Error: dup2 failed\n"), 1);
	return (var_data->pipe_check = false, 0);
}

static int	first_pipe(t_var_data *var_data, 
						int	*pipe_fd)
{
	close(STDOUT_FILENO);
	close(pipe_fd[0]);
	close(var_data->tmp_pipe[0]);
	write (var_data->std_output_fd_backup, "test1\n", 6);//TEST
	if (dup2(pipe_fd[1], STDOUT_FILENO) == -1) 
		return (ft_printf("Error: dup2 failed\n"), 1);
	return (0);
}

static int	middle_pipes(t_var_data *var_data, int *pipe_fd)
{
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(pipe_fd[0]);
	if (dup2(var_data->tmp_pipe[0], STDIN_FILENO) == -1) 
		return (ft_printf("Error: dup2 failed\n"), 1);
	write (var_data->std_output_fd_backup, "test3\n", 6);//TEST
	if (dup2(pipe_fd[1], STDOUT_FILENO) == -1) 
		return (ft_printf("Error: dup2 failed\n"), 1);
	return (0);
}

static int	last_pipe(t_var_data *var_data, int *pipe_fd)
{
	close(STDIN_FILENO);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	close(var_data->tmp_pipe[1]);
	if (dup2(var_data->tmp_pipe[0], STDIN_FILENO) == -1) 
		return (ft_printf("Error: dup2 failed\n"), 1);
	write(var_data->std_output_fd_backup, "test4\n", 6);//TEST
	/* close(var_data->previous_pipe_in); */
	return (0);
}

int	check_pipe(t_var_data *var_data, 
					t_ast_node *curr_node_pipe, 
					int *pipe_fd)
{
	struct stat info_pipefd_0;
	struct stat info_pipefd_1;

	if (!var_data->first_node_ast->pipe)
		return (0);
	if (curr_node_pipe == var_data->first_node_ast && curr_node_pipe->pipe) 
	{
		if (first_pipe(var_data, pipe_fd))
			return (1);
	}
	else if (curr_node_pipe != var_data->first_node_ast)
	{
		write (var_data->std_output_fd_backup, "test2\n", 6);//TEST
		if (curr_node_pipe->pipe)
		{
			if (middle_pipes(var_data, pipe_fd))
				return (1);
		}
		else 
			if (last_pipe(var_data, pipe_fd))
				return (1);
		var_data->pipe_check = true;
	}
	fstat(var_data->tmp_pipe[0], &info_pipefd_0);//TEST
	fstat(pipe_fd[1], &info_pipefd_1);//TEST
	if (isatty(STDIN_FILENO))
		write(var_data->std_output_fd_backup, "stdin positive\n", 15);//TEST
	else 
		dprintf(var_data->std_output_fd_backup, "pipfd0: inode %lu, size %lu\n", info_pipefd_0.st_ino, info_pipefd_0.st_size);//TEST
	if (isatty(STDOUT_FILENO))
		write(var_data->std_output_fd_backup, "stdout positive\n", 17);//TEST
	else
		dprintf(var_data->std_output_fd_backup, "pipfd1: inode %lu, size %lu\n", info_pipefd_1.st_ino, info_pipefd_0.st_size);//TEST
	write (var_data->std_output_fd_backup, "++++\n", 6);//TEST
	var_data->pipe_check = true;
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
