/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_fcts.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 18:32:45 by bclaeys           #+#    #+#             */
/*   Updated: 2024/11/18 18:47:39 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include <stdlib.h>
#include <string.h>

static char *redir_file(char *curr_token)
{
	int	i;
	char *redir_file;

	i = 0;
	while (ft_strchr("><", curr_token[i]) || ft_iswhitespace(curr_token[i]))
		i++;
	ft_printf("%d\n",i);
	ft_printf("%d\n",ft_strlen(&curr_token[i]));
	redir_file = malloc(sizeof(char) * ft_strlen(&curr_token[i]) + 1);
	if (!redir_file)
		return (NULL);
	ft_strlcpy(redir_file, &curr_token[i], ft_strlen(&curr_token[i]) + 1);
	ft_printf("%s\n", redir_file);
	return (redir_file);
}

static int redir_type(t_token_node *curr_token_node)
{
	if (!ft_strncmp(curr_token_node->token, ">>", 2))	
		return (OUTPUT_REDIR_APPEND);
	if (!ft_strncmp(curr_token_node->token, "<<", 2))	
		return (HERE_DOC);
	if (!ft_strncmp(curr_token_node->token, ">", 1))	
		return (OUTPUT_REDIR);
	if (!ft_strncmp(curr_token_node->token, "<", 1))	
		return (INPUT_REDIR);
	return (0);
}

t_ast_redir *add_redirect(t_token_node **curr_token_node, 
		t_ast_redir *first_redir_node)
{
	t_ast_redir *curr_redir_node;
	t_ast_redir *tmp_redir_node;

	tmp_redir_node = NULL;
	while (*curr_token_node && (*curr_token_node)->type == REDIRECT)
	{
		curr_redir_node = malloc(sizeof(t_ast_redir));
		if (tmp_redir_node)
			tmp_redir_node->next_redir = curr_redir_node;
		if (!first_redir_node)
			first_redir_node = curr_redir_node;
		if (!curr_redir_node)
		{
			while (curr_redir_node)
			{
				curr_redir_node = first_redir_node->next_redir;
				free(first_redir_node);
				first_redir_node = curr_redir_node;
			}
			return (NULL);	
		}
		curr_redir_node->type = redir_type(*curr_token_node);
		curr_redir_node->file = redir_file((*curr_token_node)->token);
		if (!curr_redir_node->type || !curr_redir_node->file)
			return (NULL);
		(*curr_token_node) = (*curr_token_node)->next;	
		if (!*curr_token_node || (*curr_token_node)->type != REDIRECT)
			break ;
		tmp_redir_node = curr_redir_node;
		curr_redir_node = curr_redir_node->next_redir;
	}
	curr_redir_node->next_redir = NULL;
	return (first_redir_node);
}
