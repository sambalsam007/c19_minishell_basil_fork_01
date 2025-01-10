/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_binary.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 14:09:43 by bclaeys           #+#    #+#             */
/*   Updated: 2025/01/07 19:14:02 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"
#include <dirent.h>
#include <errno.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

static int	check_pipes(t_var_data *var_data,
									t_ast_node *ast_node,
									int pipe_fd[2])
{
	if (check_pipe(var_data, ast_node, pipe_fd) || (sighandler(var_data,
				EXECUTOR)))
	{
		big_free(var_data, var_data->prmpt_to_free);
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		restore_fds(var_data);
		free_var_data(var_data);
		return (1);
	}
	return (0);
}

static int	check_builtins(t_var_data *var_data,
									t_ast_node *ast_node,
									int pipe_fd[2])
{
	if (run_builtins_with_output(var_data, ast_node))
	{
		big_free(var_data, var_data->prmpt_to_free);
		(void)pipe_fd;
		if (!isatty(pipe_fd[0]))
			close(pipe_fd[0]);
		if (!isatty(pipe_fd[1]))
			close(pipe_fd[1]);
		restore_fds(var_data);
		free_var_data(var_data);
		return (1);
	}
	return (0);
}

static void	run_binary_with_execve(t_var_data *var_data,
									char *path_bin,
									char **envvar_array,
									char **tmp_arg_array)
{
	if (execve(path_bin, tmp_arg_array, envvar_array) == -1)
	{
		ft_printf_fd(2, "Execv err: check input\n");
		var_data->error_checks->executor_level_syntax_error = true;
		free_locals_executor(path_bin, envvar_array, tmp_arg_array);
		big_free(var_data, var_data->prmpt_to_free);
		restore_fds(var_data);
		free_var_data(var_data);
		exit(1);
	}
}

static int	fork_and_execute_child(t_var_data *var_data,
									t_ast_node *ast_node,
									char *path,
									int pipe_fd[2])
{
	pid_t	pid;
	char	**envarray;
	char	**argarr;

	envarray = envvardict_to_envvararray(var_data->envvar);
	if (!envarray)
		return (1);
	argarr = add_cmd_to_argarray(ast_node->arguments, ast_node->command);
	if (tmp_argarray_error_checks(argarr, envarray, path))
		return (1);
	pid = fork();
	if (pid == -1)
		return (free_locals_executor(path, envarray, argarr), 1);
	if (pid == 0)
	{
		if (check_pipes(var_data, ast_node, pipe_fd))
			return (free_locals_executor(path, envarray, argarr), exit(1), 1);
		if (check_builtins(var_data, ast_node, pipe_fd))
			return (free_locals_executor(path, envarray, argarr), exit(0), 1);
		run_binary_with_execve(var_data, path, envarray, argarr);
	}
	else
		return (free_locals_executor(path, envarray, argarr),
			set_fds_and_continue_parent(var_data, pipe_fd));
	return (1);
}

int	execute_builtin_or_binary(t_var_data *var_data,
								t_ast_node *ast_node)
{
	char	*path_bin;
	int		pipe_fd[2];

	pipe_fd[0] = 0;
	pipe_fd[1] = 1;
	if (var_data->first_node_ast->pipe && ast_node->pipe && pipe(pipe_fd) == -1)
		return (ft_printf_fd(2, "Error: pipe failed\n"), 1);
	if (!ft_strchr("/~.", ast_node->command[0]) && !check_if_builtin(ast_node))
	{
		path_bin = check_and_create_path(var_data, ast_node->command);
		if ((!path_bin && errno == ENOENT)
			|| var_data->error_checks->executor_level_syntax_error)
			return (0);
		if (!path_bin)
			return (1);
	}
	else
		path_bin = ft_strdup(ast_node->command);
	if (path_bin && var_data->error_checks->executor_level_syntax_error == true)
		return (free(path_bin), 0);
	return (fork_and_execute_child(var_data, ast_node, path_bin, pipe_fd));
}
