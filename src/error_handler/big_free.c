/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   big_free.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 12:35:17 by bclaeys           #+#    #+#             */
/*   Updated: 2024/11/28 13:11:27 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	free_args_and_redirs(t_ast_node *tmp)
{
	int			i;
	t_ast_redir	*curr_redir;
	t_ast_redir	*tmp_redir;

	i = 0;
	if (tmp->arguments)
	{
		while (tmp->arguments[i])
			free(tmp->arguments[i++]);
		free(tmp->arguments);
		i = 0;
	}
	curr_redir = tmp->redirect;
	while (curr_redir)
	{
		tmp_redir = curr_redir->next_redir;
		free(curr_redir->file);
		free(curr_redir);
		curr_redir = tmp_redir;
	}
}

void	free_parser(t_ast_node *first_node_ast)
{
	t_ast_node	*current;
	t_ast_node	*tmp;

	if (!first_node_ast)
		return ;
	current = first_node_ast;
	while (current)
	{
		tmp = current;
		current = current->pipe;
		free_args_and_redirs(tmp);
		if (tmp->command)
			free(tmp->command);
		if (tmp->flag)
			free(tmp->flag);
		if (tmp)
			free(tmp);
		tmp = NULL;
	}
	first_node_ast = NULL;
}

void	free_lexer(t_token_node *first_node_lexer)
{
	t_token_node	*current;
	t_token_node	*tmp;

	if (!first_node_lexer)
		return ;
	current = first_node_lexer;
	while (current)
	{
		tmp = current;
		current = current->next;
		if (tmp->token)
			free(tmp->token);
		if (tmp)
			free(tmp);
		tmp = NULL;
	}
}

void	*free_var_data(t_var_data *var_data)
{
	if (var_data->envvar)
		ft_delete_dict(var_data->envvar);
	if (var_data->no_var_envvar)
		ft_delete_dict(var_data->no_var_envvar);
	if (var_data->first_node_lexer)
		free_lexer(var_data->first_node_lexer);
	if (var_data->first_node_ast)
		free_parser(var_data->first_node_ast);
	if (var_data->error_checks)
		free(var_data->error_checks);
	if (var_data)
		free(var_data);
	return (NULL);
}

void	big_free(t_var_data *var_data, char *prompt)
{
	if (prompt)
		free(prompt);
	if (var_data->first_node_lexer)
	{
		free_lexer(var_data->first_node_lexer);
		var_data->first_node_lexer = NULL;
	}
	if (var_data->first_node_ast)
	{
		free_parser(var_data->first_node_ast);
		var_data->first_node_ast = NULL;
	}
}
