/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   abstract_syntax_tree_creation_fcts.c               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 16:55:32 by bclaeys           #+#    #+#             */
/*   Updated: 2024/12/04 17:31:28 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include <stdlib.h>
#include <time.h>

void	set_all_to_null(t_ast_node *curr_ast_node)
{
	curr_ast_node->pipe = NULL;
	curr_ast_node->arguments = NULL;
	curr_ast_node->flag = NULL;
	curr_ast_node->command = NULL;
	curr_ast_node->redirect = NULL;
}

static char	**add_arguments(t_token_node **curr_token_node,
							t_token_node *tmp_curr_node,
							int i)
{
	char 			**arguments;
	
	while (tmp_curr_node && (tmp_curr_node)->type != PIPE)
	{
		if (tmp_curr_node->type == ARGUMENT)
			i++;
		tmp_curr_node = tmp_curr_node->next;
	}
	arguments = malloc(sizeof(char **) * i);
	if (!arguments)
		return (NULL);
	i = 0;
	tmp_curr_node = *curr_token_node;
	while (tmp_curr_node && tmp_curr_node->type != PIPE)
	{
		if (tmp_curr_node->type == ARGUMENT)
		{
			arguments[i] = ft_strdup(tmp_curr_node->token); 
			if (!arguments[i++])
				return (NULL);
		}
		tmp_curr_node = tmp_curr_node->next;
	}
	arguments[i] = NULL; 
	return (arguments);	
}
 
static int fill_all_values_except_command(t_token_node **curr_token_node,
								t_ast_node *new_ast_node,
								t_var_data *var_data)
{
	t_token_node 	*tmp_curr_node;
	t_ast_redir 	*tmp_redir_node;

	tmp_redir_node = NULL;
	tmp_curr_node = *curr_token_node;
	new_ast_node->arguments = add_arguments(curr_token_node, tmp_curr_node, 1);
	while ((*curr_token_node) && (*curr_token_node)->type != PIPE)
	{
		if ((*curr_token_node)->type == REDIRECT)
		{
			tmp_redir_node = add_redirect(curr_token_node, 
				tmp_redir_node, new_ast_node->redirect, var_data);
			if (!new_ast_node->redirect)
				new_ast_node->redirect = tmp_redir_node;
		}
		else if ((*curr_token_node)->type == FLAG)
			new_ast_node->flag = (*curr_token_node)->token;
		*curr_token_node = (*curr_token_node)->next;
	}
	return (0);
}

t_ast_node *create_ast_node(t_ast_node *prev_ast_node, 
							t_token_node **curr_token_node,
							t_var_data *var_data)
{
	t_ast_node	*new_ast_node;

	new_ast_node = malloc(sizeof(t_ast_node));
	if (!new_ast_node)
		return (NULL);
	set_all_to_null(new_ast_node);
	if ((*curr_token_node)->type == REDIRECT)
		if (!add_redirect(curr_token_node, NULL, 
					new_ast_node->redirect, var_data))
			return (NULL);
	new_ast_node->command = ft_strdup((*curr_token_node)->token);
	(*curr_token_node) = (*curr_token_node)->next;
	if (fill_all_values_except_command(curr_token_node, new_ast_node, var_data))
		return (NULL);
	if (prev_ast_node)
		prev_ast_node->pipe = new_ast_node;
	new_ast_node->pipe = NULL;
	return (new_ast_node);
}
