/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samd-hoo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 16:22:44 by samd-hoo          #+#    #+#             */
/*   Updated: 2025/01/09 16:22:47 by samd-hoo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"
#include <unistd.h>

int	ms_exit(t_ast_node *ast_node)
{
	int	i;

	if (ast_node->pipe == NULL)
	{
		i = 0;
		if (ast_node->arguments[0] == NULL)
		{
			exit (0);
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
		exit (ft_atoi(ast_node->arguments[0]));
	}
	return (0);
}
