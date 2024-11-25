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
		t_var_data *vardata)
{
	t_token_node 	*current_token;
	t_ast_node 		*current_ast_node;
	t_ast_node 		*prev_ast_node;

	current_token = first_token_node;
	*first_ast_node = create_ast_node(NULL, &current_token, vardata);
	if (!*first_ast_node)
		return (1);
	if (prompt_error_checks(first_token_node, vardata->error_checks))
		return (0);
	current_ast_node = *first_ast_node;
	while (current_token)
	{
		prev_ast_node = current_ast_node;
		current_ast_node = current_ast_node->pipe;
		if (current_token->type == PIPE && !current_token->next)
			return(ft_printf("Error: '|' expects command\n"), 
					vardata->error_checks->parser_level_syntax_error = true, 0);
		current_token = current_token->next;
		current_ast_node = create_ast_node(prev_ast_node, &current_token, vardata);
		if (!current_ast_node)
			return (1);
		prev_ast_node->pipe = current_ast_node;
	}
	return (0);
}
