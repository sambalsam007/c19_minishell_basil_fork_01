#include "../../minishell.h"
#include <readline/history.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define ERROR_CONTINUE -1
#define ERROR_STOP 1

// xxx new
int	handle_pipes(t_var_data *var_data)
{
	if (var_data->first_node_ast->pipe)
	{
		if (pipe(var_data->tmp_pipe) == ERROR_CONTINUE)
		{
			ft_putstr_fd("Error: pipe failed\n", STDERR_FILENO);
			return (ERROR_STOP);
		}
	}
	return (0);
}

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
}

int	execute_logic(t_var_data *var_data)
{
	t_ast_node	*tmp_node;
	int			error_flow;
	int			status;
	int			traversal_result;

	tmp_node = var_data->first_node_ast;
	error_flow = 0;
	if (handle_pipes(var_data) == ERROR_STOP) // xxx
		return (ERROR_STOP);
	traversal_result = traverse_ast(tmp_node, var_data, error_flow); // xxx
	if (traversal_result != 2)
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
