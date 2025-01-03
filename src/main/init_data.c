/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 15:18:48 by bclaeys           #+#    #+#             */
/*   Updated: 2024/12/20 17:25:05 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	***init_envvar_noenvp(void)
{
	char	*temp[4];

	// temp[0] aanpassen met getcwd()
	temp[0] = "PWD=/nfs/homes/bclaeys/common_core/minishell";
	temp[1] = "SHLVL=1";
	temp[2] = "_=/usr/bin/env";
	temp[3] = NULL;
	return (ft_create_dict(temp, '='));
}

char	***init_envvar_list(char **envp)
{
	if (!envp[0])
		return (init_envvar_noenvp());
	return (ft_create_dict(envp, '='));
}

void	init_error_data(t_var_data *var_data, t_error_checks *error_checks)
{
	if (error_checks->lexer_level_syntax_error == true)
		var_data->last_error_code = LEXER_ERROR;
	if (error_checks->parser_level_syntax_error == true)
		var_data->last_error_code = PARSER_ERROR;
	if (error_checks->executor_level_syntax_error == true)
		var_data->last_error_code = EXECUTOR_ERROR;
	error_checks->lexer_level_syntax_error = false;
	error_checks->parser_level_syntax_error = false;
	error_checks->executor_level_syntax_error = false;
}

int	backup_fds(t_var_data *var_data)
{
	var_data->std_input_fd_backup = dup(STDIN_FILENO);
	if (var_data->std_input_fd_backup == -1)
		return (ft_putstr_fd("Error: dup failed\n", STDERR_FILENO), 1);
	var_data->std_output_fd_backup = dup(STDOUT_FILENO);
	if (var_data->std_output_fd_backup == -1)
		return (ft_putstr_fd("Error: dup failed\n", STDERR_FILENO), 1);
	return (0);
}

t_var_data	*init_var_data(char **envp)
{
	t_var_data		*var_data;
	t_error_checks	*error_checks;

	var_data = malloc(sizeof(t_var_data));
	error_checks = malloc(sizeof(t_error_checks));
	if (!var_data)
		return (ft_putstr_fd("Error: malloc failed\n", 3), NULL);
	if (!(var_data->envvar = init_envvar_list(envp)))
		return (free_var_data(var_data), ft_putstr_fd("Error:mall\n", 3), NULL);
	if (backup_fds(var_data))
		return (free_var_data(var_data), NULL);
	ft_update_dict("SHLVL", ft_itoa(ft_atoi(ft_get_value("SHLVL", 
						var_data->envvar)) + 1), var_data->envvar);
	var_data->error_checks = error_checks;
	error_checks->lexer_level_syntax_error = false;
	error_checks->parser_level_syntax_error = false;
	error_checks->executor_level_syntax_error = false;
	var_data->first_node_lexer = NULL;
	var_data->first_node_ast = NULL;
	var_data->no_var_envvar = NULL;
	var_data->open_input_file_fd = -1;
	var_data->open_output_file_fd = -1;
	var_data->last_error_code = 0;
	var_data->pipe_check = false;
	var_data->termios_backup_check = false;
	var_data->is_redirect = false;
	return (var_data);
}
