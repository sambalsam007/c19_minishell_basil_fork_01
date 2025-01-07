/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 16:07:35 by bclaeys           #+#    #+#             */
/*   Updated: 2025/01/07 13:14:25 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	parser(t_ast_node **first_ast_node,
			t_token_node *first_token_node,
			t_var_data *var_data)
{
	t_token_node	*curr_token;
	t_ast_node		*curr_ast_node;
	t_ast_node		*prev_ast_node;

	curr_token = first_token_node;
	*first_ast_node = create_ast_node(NULL, &curr_token, var_data);
	if (!*first_ast_node)
		return (1);
	if (prompt_error_checks(first_token_node, var_data->error_checks))
		return (0);
	curr_ast_node = *first_ast_node;
	while (curr_token)
	{
		prev_ast_node = curr_ast_node;
		curr_ast_node = curr_ast_node->pipe;
		if (curr_token->type == PIPE && !curr_token->next)
			return (var_data->error_checks->parser_level_syntax_error = true,
				ft_printf("Error: '|' expects command\n"), 0);
		curr_token = curr_token->next;
		curr_ast_node = create_ast_node(prev_ast_node, &curr_token, var_data);
		if (!curr_ast_node)
			return (1);
		prev_ast_node->pipe = curr_ast_node;
	}
	return (0);
}
