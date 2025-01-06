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

static char	*replace_filename_w_envvar(char *token_string, t_var_data *var_data)
{
	char	*tmp;
	char	*key;
	char	*value;

	tmp = token_string;
	key = ft_get_key(&(token_string)[1]);
	value = ft_get_value(key, var_data->envvar);
	if (!value)
		return (var_data->error_checks->parser_level_syntax_error = true,
				ft_printf("Error: no such file or directory\n"),
				free(key),
				token_string);
	token_string = ft_strdup(value);
	free(tmp);
	free(key);
	return (token_string);
}

static char	*redir_file(char *curr_token, t_var_data *var_data)
{
	int		i;
	char	*redir_file;

	i = 0;
	while (ft_strchr("><", curr_token[i]) || ft_iswhitespace(curr_token[i]))
		i++;
	redir_file = malloc(sizeof(char) * ft_strlen(&curr_token[i]) + 1);
	if (!redir_file)
		return (NULL);
	ft_strlcpy(redir_file, &curr_token[i], ft_strlen(&curr_token[i]) + 1);
	if (redir_file[0] == '$')
		redir_file = replace_filename_w_envvar(redir_file, var_data);
	return (redir_file);
}

static int	redir_type(t_token_node *curr_token_node)
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

t_ast_redir	*add_redirect(t_token_node **curr_token_node,
							t_ast_redir *curr_redir_node,
							t_ast_redir *first_redir_node,
							t_var_data *var_data)
{
	t_ast_redir	*new_redir;

	new_redir = malloc(sizeof(t_ast_redir));
	if (!new_redir)
	{
		new_redir = first_redir_node;
		while (new_redir->next_redir)
		{
			free(new_redir);
			new_redir = first_redir_node->next_redir;
		}
		first_redir_node = NULL;
		return (NULL);
	}
	new_redir->type = redir_type(*curr_token_node);
	new_redir->file = redir_file((*curr_token_node)->token, var_data);
	if (!new_redir->type || !new_redir->file)
		return (NULL);
	if (curr_redir_node)
		curr_redir_node->next_redir = new_redir;
	new_redir->next_redir = NULL;
	return (new_redir);
}
