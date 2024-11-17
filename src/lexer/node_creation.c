/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_creation.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 15:55:55 by bclaeys           #+#    #+#             */
/*   Updated: 2024/11/15 16:55:48 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	type_giver(char *prompt, t_token_node *prev_node)
{
	int				i;
	t_token_node	*tmp_node;

	i = 0;
	tmp_node = prev_node;
	if (!prompt)
		return (-1);
	if (prompt[i] == '|')
		return (PIPE);
	if (prompt[i] == '-' && prompt[i + 1] == 'n')
		return (FLAG);
	if (prompt[i] == '<' || prompt[i] == '>')
		return (REDIRECT);
	if ((prev_node && prev_node->type == EXEC) ||
		prompt[i] == 39 || prompt[i] == 34)
		return (ARGUMENT);
	while (tmp_node && tmp_node->type != PIPE)
	{
		if (tmp_node->type == EXEC)
			return (ARGUMENT);
		tmp_node = tmp_node->prev;
	}
	return (EXEC);
}

t_token_node	*create_node(char *tokenized_str, 
					t_token_node *prev_node,
					t_token_node *current_node)
{
	t_token_node	*token_node;

	if (!current_node)
	{
		token_node = malloc(sizeof(t_token_node));
		if (!token_node)
			return (ft_print_error_null("Error: malloc failed\n"));
		token_node->type = 0;
		token_node->prev = NULL;
	}
	else
		token_node = current_node;
	token_node->token = tokenized_str;
	if (token_node->type == 0)
		token_node->type = type_giver(tokenized_str, prev_node);
	token_node->argument_check = true;
	if (prev_node && prev_node != current_node)
	{
		prev_node->next = token_node;
		token_node->prev = prev_node;
	}
	token_node->next = NULL;
	return (token_node);
}
