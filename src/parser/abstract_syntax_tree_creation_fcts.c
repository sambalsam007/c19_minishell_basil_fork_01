/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   abstract_syntax_tree_creation_fcts.c               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 16:55:32 by bclaeys           #+#    #+#             */
/*   Updated: 2024/11/18 18:51:11 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include <stdlib.h>

void	set_all_to_null(t_ast_node *curr_ast_node)
{
	curr_ast_node->pipe = NULL;
	curr_ast_node->arguments = NULL;
	curr_ast_node->flag = NULL;
	curr_ast_node->command = NULL;
	curr_ast_node->redirect = NULL;
}

static char	**add_arguments(t_token_node **curr_token_node)
{
	int				i;
	char 			**arguments;
	t_token_node 	*tmp_curr_node;
	
	i = 1;
	tmp_curr_node = *curr_token_node;
	while (tmp_curr_node && (tmp_curr_node)->type == ARGUMENT)
	{
		tmp_curr_node = tmp_curr_node->next;
		i++;
	}
	arguments = malloc(sizeof(char **) * i);
	i = 0;
	while (*curr_token_node && (*curr_token_node)->type == ARGUMENT)
	{
		arguments[i] = ft_strdup((*curr_token_node)->token); 
		if (!arguments)
			return (NULL);
		i++;
		(*curr_token_node) = (*curr_token_node)->next;
	}
	arguments[i] = NULL; 
	return (arguments);	
}
 
static int fill_all_values_except_command(t_token_node **curr_token_node,
							t_ast_node *new_ast_node)
{
	while ((*curr_token_node) && (*curr_token_node)->type != PIPE)
	{
		if ((*curr_token_node)->type == REDIRECT)
		{
			new_ast_node->redirect = add_redirect(curr_token_node, 
					new_ast_node->redirect);
			if (!new_ast_node->redirect)
				return (1);
		}
		else if ((*curr_token_node)->type == FLAG)
			new_ast_node->flag = (*curr_token_node)->token;
		else if ((*curr_token_node)->type == ARGUMENT)
			new_ast_node->arguments = add_arguments(curr_token_node);
		else 
			*curr_token_node = (*curr_token_node)->next;
	}
	return (0);
}

t_ast_node *create_ast_node(t_ast_node *prev_ast_node, 
							t_token_node **curr_token_node)
{
	t_ast_node	*new_ast_node;

	new_ast_node = malloc(sizeof(t_ast_node));
	if (!new_ast_node)
		return (NULL);
	set_all_to_null(new_ast_node);
	if ((*curr_token_node)->type == REDIRECT)
		if (!add_redirect(curr_token_node, new_ast_node->redirect))
			return (NULL);
	new_ast_node->command = ft_strdup((*curr_token_node)->token);
	(*curr_token_node) = (*curr_token_node)->next;
	if (fill_all_values_except_command(curr_token_node, new_ast_node))
		return (NULL);
	if (prev_ast_node)
		prev_ast_node->pipe = new_ast_node;
	new_ast_node->pipe = NULL;
	return (new_ast_node);
}
