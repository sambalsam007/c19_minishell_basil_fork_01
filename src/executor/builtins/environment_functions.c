/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_functions.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 15:41:17 by bclaeys           #+#    #+#             */
/*   Updated: 2025/01/07 13:17:21 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

int	ms_env(t_var_data *var_data, t_ast_node *ast_node)
{
	if (ast_node->arguments[0])
	{
		var_data->error_checks->parser_level_syntax_error = true;
		ft_printf("Error: arguments\n", 2);
		return (1);
	}
	ft_print_dict(var_data->envvar, "=", 1);
	return (0);
}

int	ms_unset(t_var_data *var_data, t_ast_node *ast_node)
{
	char	***envvar_new;
	int		i;

	i = 0;
	if (!ast_node->arguments)
	{
		var_data->error_checks->parser_level_syntax_error = true;
		return (ft_printf("Error: no arguments\n"), 2);
	}
	while (ast_node->arguments[i])
	{
		if (!ft_get_value(ast_node->arguments[i], var_data->envvar))
			return (ft_printf("Error: variable not found\n"), 2);
		envvar_new = ft_delete_from_dict(ast_node->arguments[i],
				var_data->envvar);
		if (!envvar_new)
			return (ft_printf("Error: malloc\n"), 2);
		i++;
		var_data->envvar = envvar_new;
	}
	return (0);
}
