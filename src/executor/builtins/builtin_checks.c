/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_checks.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 12:49:56 by bclaeys           #+#    #+#             */
/*   Updated: 2024/11/30 14:16:40 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

int	check_if_builtin(t_var_data *var_data, t_ast_node *ast_node)
{
	size_t	cmd_len;

	if (!ast_node->command)
		return (1);
	cmd_len = ft_strlen(ast_node->command);
	if (!ft_strncmp("echo", ast_node->command, cmd_len))
		ms_echo(var_data, ast_node);
	else if (!ft_strncmp("env", ast_node->command, cmd_len))
		ms_env(var_data, ast_node);
	else if (!ft_strncmp("export", ast_node->command, cmd_len))
		ms_export(var_data, ast_node);
	else if (!ft_strncmp("unset", ast_node->command, cmd_len))
		ms_unset(var_data, ast_node);
	else if (!ft_strncmp("pwd", ast_node->command, cmd_len))
		ms_pwd(var_data);
	else if (!ft_strncmp("cd", ast_node->command, cmd_len))
		ms_cd(var_data, ast_node);
	else 
		return (-1);
	return (0);
}
