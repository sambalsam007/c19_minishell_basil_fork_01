/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 16:34:43 by bclaeys           #+#    #+#             */
/*   Updated: 2025/01/04 17:15:30 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

int	restore_fds(t_var_data *var_data)
{
	if (dup2(var_data->std_input_fd_backup, STDIN_FILENO) == -1)
		return (ft_putstr_fd("Error: dup2 failed\n", STDERR_FILENO), 1);
	if (dup2(var_data->std_output_fd_backup, STDOUT_FILENO) == -1)
		return (ft_putstr_fd("Error: dup2 failed\n", STDERR_FILENO), 1);
	return (var_data->pipe_check = false, 0);
}

static int	first_pipe(int *pipe_fd)
{
	close(pipe_fd[0]);
	if (isatty(STDOUT_FILENO) && dup2(pipe_fd[1], STDOUT_FILENO) == -1)
		return (ft_putstr_fd("Error: dup2 failed\n", STDERR_FILENO), 1);
	close(pipe_fd[1]);
	return (0);
}

static int	middle_pipes(t_var_data *var_data, int *pipe_fd)
{
	close(pipe_fd[0]);
	if (isatty(STDIN_FILENO) && dup2(var_data->tmp_pipe[0], STDIN_FILENO) == -1)
		return (ft_putstr_fd("Error: dup2 failed\n", STDERR_FILENO), 1);
	if (isatty(STDOUT_FILENO) && dup2(pipe_fd[1], STDOUT_FILENO) == -1)
		return (ft_putstr_fd("Error: dup2 failed\n", STDERR_FILENO), 1);
	close(pipe_fd[1]);
	return (0);
}

static int	last_pipe(t_var_data *var_data)
{
	if (dup2(var_data->tmp_pipe[0], STDIN_FILENO) == -1)
		return (ft_putstr_fd("Error: dup2 failed\n", STDERR_FILENO), 1);
	close(var_data->tmp_pipe[0]);
	return (0);
}

/* static void test_pipes(t_var_data *var_data, */
/* 						int *pipe_fd) */
/* { */
/* 	struct stat info_pipefd_0; */
/* 	struct stat info_pipefd_1; */
/**/
/* 	fstat(var_data->tmp_pipe[0], &info_pipefd_0);//TEST */
/* 	fstat(pipe_fd[1], &info_pipefd_1);//TEST */
/* 	if (isatty(STDIN_FILENO)) */
/* 		write(var_data->std_output_fd_backup, "stdin positive\n", 15);//TEST */
/* 	else  */
/* 		dprintf(var_data->std_output_fd_backup, "pipfd0: inode %lu, size %lu\n",
				info_pipefd_0.st_ino, info_pipefd_0.st_size);//TEST */
/* 	if (isatty(STDOUT_FILENO)) */
/* 		write(var_data->std_output_fd_backup, "stdout positive\n", 17);//TEST */
/* 	else */
/* 		dprintf(var_data->std_output_fd_backup, "pipfd1: inode %lu, size %lu\n",
				info_pipefd_1.st_ino, info_pipefd_0.st_size);//TEST */
/* 	write (var_data->std_output_fd_backup, "++++\n", 6);//TEST */
/* } */

int	check_pipe(t_var_data *var_data,
				t_ast_node *curr_node_pipe,
				int *pipe_fd)
{
	if (!var_data->first_node_ast->pipe)
		return (0);
	if (curr_node_pipe == var_data->first_node_ast && curr_node_pipe->pipe)
	{
		if (first_pipe(pipe_fd))
			return (1);
	}
	else if (curr_node_pipe != var_data->first_node_ast)
	{
		if (curr_node_pipe->pipe)
		{
			if (middle_pipes(var_data, pipe_fd))
				return (1);
		}
		else if (last_pipe(var_data))
			return (1);
		var_data->pipe_check = true;
	}
	var_data->pipe_check = true;
	return (0);
}
