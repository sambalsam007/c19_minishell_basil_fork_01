/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_checks.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 12:49:56 by bclaeys           #+#    #+#             */
/*   Updated: 2025/01/04 19:14:41 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"
#include <unistd.h>

/* static int builtin_pipes(t_var_data *var_data, t_ast_node *ast_node) */
/* { */
/* 	int		pipe_fd[2]; */
/**/
/* 	pipe_fd[0] = 0; */
/* 	pipe_fd[1] = 1; */
/* 	close(STDOUT_FILENO); */
/* 	close(pipe_fd[0]); */
/* 	if (dup2(var_data->tmp_pipe[0], STDIN_FILENO) == -1)  */
/* 		return (ft_putstr_fd("Error: dup2 failed\n", STDERR_FILENO), 1); */
/* 	if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)  */
/* 		return (ft_putstr_fd("Error: dup2 failed\n", STDERR_FILENO), 1); */
/* 	return(0); */
/* } */

int	check_if_builtin(t_ast_node *ast_node)
{
	size_t	cmd_len;

	cmd_len = ft_strlen(ast_node->command);
	if (!ft_strncmp("echo", ast_node->command, cmd_len))
		return (1);
	else if (!ft_strncmp("export", ast_node->command, cmd_len))
		return (1);
	else if (!ft_strncmp("env", ast_node->command, cmd_len))
		return (1);
	else if (!ft_strncmp("unset", ast_node->command, cmd_len))
		return (1);
	else if (!ft_strncmp("pwd", ast_node->command, cmd_len))
		return (1);
	else if (!ft_strncmp("cd", ast_node->command, cmd_len))
		return (1);
	return (0);
}

int	run_builtins_without_output(t_var_data *var_data, t_ast_node *ast_node)
{
	size_t	cmd_len;

	cmd_len = ft_strlen(ast_node->command);
	if (!ft_strncmp("export", ast_node->command, cmd_len)
		&& ast_node->arguments)
		ms_export(var_data, ast_node);
	else if (!ft_strncmp("unset", ast_node->command, cmd_len))
		ms_unset(var_data, ast_node);
	else if (!ft_strncmp("cd", ast_node->command, cmd_len))
		ms_cd(var_data, ast_node);
	else
		return (0);
	return (-1);
}

int	run_builtins_with_output(t_var_data *var_data, t_ast_node *ast_node)
{
	size_t	cmd_len;

	cmd_len = ft_strlen(ast_node->command);
	if (!ft_strncmp("echo", ast_node->command, cmd_len))
		ms_echo(var_data, ast_node);
	else if (!ft_strncmp("export", ast_node->command, cmd_len))
		ms_export(var_data, ast_node);
	else if (!ft_strncmp("pwd", ast_node->command, cmd_len))
		ms_pwd(var_data);
	else if (!ft_strncmp("env", ast_node->command, cmd_len))
		ms_env(var_data, ast_node);
	else
		return (0);
	return (1);
}
