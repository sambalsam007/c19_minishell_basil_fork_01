/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 16:07:35 by bclaeys           #+#    #+#             */
/*   Updated: 2024/11/18 19:03:01 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	parser(t_ast_node **first_ast_node, 
		t_token_node *first_token_node, 
		t_var_data *var_data)
{
	t_token_node 	*current_token_node;
	t_ast_node 		*current_ast_node;
	t_ast_node 		*prev_ast_node;

	current_token_node = first_token_node;
	*first_ast_node = create_ast_node(NULL, &current_token_node);
	if (!*first_ast_node)
		return (1);
	if (prompt_error_checks(first_token_node, var_data->error_checks))
		return (-1);
	current_ast_node = *first_ast_node;
	while (current_token_node)
	{
		prev_ast_node = current_ast_node;
		current_ast_node = current_ast_node->pipe;
		current_token_node = current_token_node->next;
		current_ast_node = create_ast_node(prev_ast_node, &current_token_node);
		if (!current_ast_node)
			return (1);
		prev_ast_node->pipe = current_ast_node;
	}
	return (0);
}
