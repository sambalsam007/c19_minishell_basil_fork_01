<<<<<<< HEAD
=======
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_logic.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 13:11:59 by bclaeys           #+#    #+#             */
/*   Updated: 2025/01/07 14:53:59 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

>>>>>>> upstream/master
#include "../../minishell.h"
#include <readline/history.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define ERROR_CONTINUE -1
#define ERROR_STOP 1

<<<<<<< HEAD
// xxx new
int	handle_pipes(t_var_data *var_data)
{
	if (var_data->first_node_ast->pipe)
	{
		if (pipe(var_data->tmp_pipe) == ERROR_CONTINUE)
		{
			ft_putstr_fd("Error: pipe failed\n", STDERR_FILENO);
=======
static int	handle_pipes(t_var_data *var_data)
{
	if (var_data->first_node_ast->pipe)
	{
		if (pipe(var_data->tmp_pipe) == -1)
		{
			ft_printf_fd(2, "Err: pipe failed\n");
>>>>>>> upstream/master
			return (ERROR_STOP);
		}
	}
	return (0);
}

<<<<<<< HEAD
// xxx new
int	traverse_ast(t_ast_node *tmp_node, t_var_data *var_data, int error_flow)
{
	while (tmp_node)
	{
		if (!tmp_node->command)
			return (var_data->error_checks->executor_level_syntax_error = true,
				ft_putstr_fd("Error: no command\n", STDERR_FILENO), 0);
		error_flow = check_if_redir(var_data, tmp_node->redirect);
		if (error_flow)
			return (ERROR_STOP);
		if (error_flow == ERROR_CONTINUE)
			return (2);
		error_flow = check_if_builtin(var_data, tmp_node);
		if (error_flow == ERROR_STOP)
			return (ERROR_STOP);
		if (error_flow == ERROR_CONTINUE && check_if_binary(var_data, tmp_node))
			return (ERROR_STOP);
		tmp_node = tmp_node->pipe;
	}
	return (2);
=======
static int	traverse_ast(t_ast_node *tmp_node, t_var_data *var_data)
{
	int	error_flow;

	error_flow = 0;
	while (tmp_node)
	{
		error_flow = check_if_redir(var_data, tmp_node->redirect, 0,
				tmp_node->command);
		if (!tmp_node->command)
			return (var_data->error_checks->executor_level_syntax_error = true,
				ft_printf_fd(2, "Err: no command\n"), 0);
		if (error_flow == ERROR_CONTINUE
			|| var_data->error_checks->executor_level_syntax_error)
			return (ERROR_CONTINUE);
		if (error_flow == ERROR_STOP || var_data->error_checks->fatal_error)
			return (ERROR_STOP);
		error_flow = run_builtins_without_output(var_data, tmp_node);
		if (!error_flow)
		{
			error_flow = execute_builtin_or_binary(var_data, tmp_node);
			if (error_flow == ERROR_STOP)
				return (ERROR_STOP);
		}
		tmp_node = tmp_node->pipe;
	}
	return (0);
>>>>>>> upstream/master
}

int	execute_logic(t_var_data *var_data)
{
	t_ast_node	*tmp_node;
<<<<<<< HEAD
	int			error_flow;
=======
>>>>>>> upstream/master
	int			status;
	int			traversal_result;

	tmp_node = var_data->first_node_ast;
<<<<<<< HEAD
	error_flow = 0;
	if (handle_pipes(var_data) == ERROR_STOP) // xxx
		return (ERROR_STOP);
	traversal_result = traverse_ast(tmp_node, var_data, error_flow); // xxx
	if (traversal_result != 2)
=======
	if (handle_pipes(var_data) == ERROR_STOP)
		return (ERROR_STOP);
	traversal_result = traverse_ast(tmp_node, var_data);
	if (traversal_result != 0)
>>>>>>> upstream/master
		return (traversal_result);
	while (wait(&status) > 0)
	{
		if (WIFEXITED(status))
			var_data->last_error_code = WEXITSTATUS(status);
		else
			var_data->last_error_code = 0;
	}
	return (0);
}
