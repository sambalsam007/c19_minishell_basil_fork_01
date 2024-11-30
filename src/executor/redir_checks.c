/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_checks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 13:49:12 by bclaeys           #+#    #+#             */
/*   Updated: 2024/11/30 15:59:09 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	check_if_redir(t_var_data *var_data, t_ast_node *curr_node, int *fd)
{
	t_ast_redir	*tmp;

	if (!curr_node->redirect)
		return (0);
	tmp = curr_node->redirect;
	/* ik geef een fd mee aan mijn executor, die ik hier aanpas, maar hoe handle ik dan  */
	/* meerderere redirects? */
	while (tmp)
	{
		//logic	
		tmp = curr_node->redirect->next_redir;
	}
	return (0);
}
