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

/* void	test_print_parser2(t_var_data *var_data) */
/* { */
/* 	int				i; */
/* 	t_ast_node		*tmp; */
/* 	t_ast_redir		*tmp_redir; */
/**/
/* 	i = 0; */
/* 	tmp = var_data->first_node_ast; */
/* 	while (tmp) */
/* 	{ */
/* 		ft_printf("command: %s\n", tmp->command); */
/* 		while (tmp->arguments && tmp->arguments[i]) */
/* 		{ */
/* 			ft_printf("arguments: %s\n", tmp->arguments[i]); */
/* 			i++;		*/
/* 		} */
/* 		i = 0; */
/* 		tmp_redir = tmp->redirect; */
/* 		while (tmp_redir) */
/* 		{ */
/* 			ft_printf("redirects: type = %d, file = %s\n",  */
/* 					tmp_redir->type, tmp_redir->file); */
/* 			tmp_redir = tmp_redir->next_redir; */
/* 		} */
/* 		tmp = tmp->pipe; */
/* 		ft_printf("+++++\n"); */
/* 	} */
/* } */

static void	check_pipes_and_builtins(t_var_data *var_data,
									t_ast_node *ast_node,
									int pipe_fd[2])
{
	if (check_pipe(var_data, ast_node, pipe_fd) || (sighandler(var_data,
				EXECUTOR)))
		exit(1);
	if (run_builtins_with_output(var_data, ast_node))
		exit(0);
}

static void	run_binary_with_execve(t_var_data *var_data,
									char *path_bin,
									char **envvar_array,
									char **tmp_arg_array)
{
	if (execve(path_bin, tmp_arg_array, envvar_array) == -1)
	{
		var_data->error_checks->executor_level_syntax_error = true;
		free_path_and_arrays(path_bin, envvar_array, tmp_arg_array);
		ft_putstr_fd("Execv err: check input\n", STDERR_FILENO);
		exit(1);
	}
}

static int	fork_and_execute_child(t_var_data *var_data,
									t_ast_node *ast_node,
									char *path_bin,
									int pipe_fd[2])
{
	pid_t	pid;
	char	**envvar_array;
	char	**tmp_arg_array;

	envvar_array = envvardict_to_envvararray(var_data->envvar);
	if (!envvar_array)
		return (1);
	tmp_arg_array = add_cmd_to_argarray(ast_node->arguments, ast_node->command);
	if (tmp_argarray_error_checks(tmp_arg_array, envvar_array, path_bin))
		return (1);
	pid = fork();
	if (pid == -1)
		return (free_path_and_arrays(path_bin, envvar_array, tmp_arg_array),
			ft_putstr_fd("Error: couldn't fork\n", STDERR_FILENO), 1);
	if (pid == 0)
	{
		check_pipes_and_builtins(var_data, ast_node, pipe_fd);
		run_binary_with_execve(var_data, path_bin, envvar_array, tmp_arg_array);
	}
	else
	{
		free_path_and_arrays(path_bin, envvar_array, tmp_arg_array);
		return (set_fds_and_continue_parent(var_data, ast_node, pipe_fd));
	}
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
		return (ft_putstr_fd("Error: pipe failed\n", STDERR_FILENO), 1);
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
