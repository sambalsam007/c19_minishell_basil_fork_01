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

#include "../../minishell.h"
#include <readline/history.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define ERROR_CONTINUE -1
#define ERROR_STOP 1

static int	handle_pipes(t_var_data *var_data)
{
	if (var_data->first_node_ast->pipe)
	{
		if (pipe(var_data->tmp_pipe) == -1)
		{
			ft_printf_fd(2, "Err: pipe failed\n");
			return (ERROR_STOP);
		}
	}
	return (0);
}

static int	traverse_ast(t_ast_node *tmp_node, t_var_data *var_data)
{
	int	error_flow;

	error_flow = 0;
	while (tmp_node)
	{
		error_flow = check_if_redir(var_data, tmp_node->redirect, 0,
				tmp_node->command);
			// check: is het redir, of heredoc
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
}

int	execute_logic(t_var_data *var_data)
{
	t_ast_node	*tmp_node;
	int			status;
	int			traversal_result;

	tmp_node = var_data->first_node_ast;
	if (handle_pipes(var_data) == ERROR_STOP)
		return (ERROR_STOP);
	traversal_result = traverse_ast(tmp_node, var_data);
	if (traversal_result != 0)
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
