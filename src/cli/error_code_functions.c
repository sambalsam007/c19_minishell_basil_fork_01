/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_code_functions.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 16:37:24 by bclaeys           #+#    #+#             */
/*   Updated: 2024/12/20 16:59:46 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*expand_error_code_variable(char *argument, char *error_code,
									int i, int j)
{
	char	*new_arg;

	while (argument[i])
	{
		if (argument[i] == '$')
			i += ft_strlen(error_code);
		else
			i++;
	}
	new_arg = malloc(sizeof(char) * i);
	if (!new_arg)
		return (free(argument), free(error_code), NULL);
	i = 0;
	while (argument[i])
	{
		if (argument[i] == '$')
		{
			while (error_code[j])
				new_arg[i++] = error_code[j++];
			i++;
		}
		else
			new_arg[j++] = argument[i++];
	}
	return (new_arg[j] = 0, free(argument), free(error_code), new_arg);
}

int	expand_error_codes(t_var_data *var_data)
{
	int			i;
	t_ast_node	*tmp_ast_node;

	i = 0;
	tmp_ast_node = var_data->first_node_ast;
	while (tmp_ast_node)
	{
		while (tmp_ast_node->arguments[i])
		{
			if (ft_strnstr(tmp_ast_node->arguments[i], "$?",
					ft_strlen(tmp_ast_node->arguments[i])))
			{
				tmp_ast_node->arguments[i]
					= expand_error_code_variable(tmp_ast_node->arguments[i],
						ft_itoa(var_data->last_error_code), 0, 0);
				if (!tmp_ast_node->arguments[i])
					return (ft_putstr_fd("Error: malloc\n", STDERR_FILENO), 1);
			}
			i++;
		}
		tmp_ast_node = tmp_ast_node->pipe;
	}
	return (0);
}
