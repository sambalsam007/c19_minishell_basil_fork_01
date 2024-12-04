/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 12:49:12 by bclaeys           #+#    #+#             */
/*   Updated: 2024/11/25 13:21:26 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

int	ms_echo(t_var_data *var_data, t_ast_node *ast_node)
{
	int		i;
	bool	flag_check;

	i = 0;
	flag_check = false;
	if (!ast_node->arguments)
		ft_putstr_fd("\n", 1);
	while (ast_node->arguments[i])		
	{
		if (!ft_strncmp(ast_node->arguments[i], "-n", 3))
			flag_check = true;
		else
			ft_putstr_fd(ast_node->arguments[i], 1);
		i++;
		write(1, " ", 1);
	}
	if (flag_check != true)
		write(1, "\n", 1);
	(void)var_data;
	return (0);
}
