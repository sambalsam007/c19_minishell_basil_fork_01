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
static int	check_other_letters(t_ast_node *ast_node, int i)	
{
	int	j;

	j = 1;
	while (ast_node->arguments[i][j])
	{
		if (ast_node->arguments[i][j] != 'n')
			return (1);
		j++;
	}
	return (0);
}

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
		if (!ft_strncmp(ast_node->arguments[i], "-n", 2)
				&& !check_other_letters(ast_node, i))
			flag_check = true;
		else
		{
			ft_putstr_fd(ast_node->arguments[i], 1);
			if (ast_node->arguments[i+1])
				write(1, " ", 1);
		}
		i++;
	}
	if (flag_check != true)
		write(1, "\n", 1);
	(void)var_data;
	return (0);
}
