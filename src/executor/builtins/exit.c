/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samd-hoo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 16:22:44 by samd-hoo          #+#    #+#             */
/*   Updated: 2025/01/10 17:01:25 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"
#include <unistd.h>

int	free_before_exit(t_var_data *var_data)
{
	big_free(var_data, var_data->prmpt_to_free);
	restore_tty(var_data);
	restore_fds(var_data);
	free_var_data(var_data);
	return (0);
}

int	ms_exit(t_ast_node *ast_node, t_var_data *var_data)
{
	int	i;

	if (ast_node->pipe == NULL)
	{
		i = 0;
		if (ast_node->arguments[0] == NULL)
		{
			free_before_exit(var_data);
			exit(0);
		}
		while (ast_node->arguments[0][i])
		{
			if (!ft_isdigit(ast_node->arguments[0][i]) && \
					ast_node->arguments[0][i] != '-')
				return (ft_printf_fd(2, "Err: non-numerical arg.\n"), 2);
			i++;
		}
		if (ast_node->arguments[1])
			return (ft_printf_fd(2, "Err: too many args.\n"), 2);
		i = ft_atoi(ast_node->arguments[0]);
		free_before_exit(var_data);
		exit(i);
	}
	return (0);
}
