/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 11:47:45 by bclaeys           #+#    #+#             */
/*   Updated: 2024/11/15 18:00:28 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <time.h>

int	whitespace_exception(char *prompt, 
						size_t *index, 
						char ***envvar,
						char **token)
{
	int		check;

	check = 0;
	if (ft_strchr("><", prompt[*index]))
		*token = redirect_handler(prompt, index);
	else if (ft_strchr("'", prompt[*index]))
		check = single_quotes(prompt, index, token);
	/* else if (ft_strchr("$", prompt[*index])  */
	/* 		&& (ft_iswhitespace(prompt[*index + 1]) || !prompt[*index + 1])) */
	/* { */
	/* 	*index += 1; */
	/* 	*token = ft_strdup("$"); */
	/* } */
	else if (ft_strchr("$", prompt[*index]))
		while (prompt[*index] && ft_strchr("$", prompt[*index]) && check != 2)
			check = no_quotes_arg(prompt, index, envvar, token);
	else if (prompt[*index] == '"')
		check = double_quotes(prompt, index, envvar, token);
	else
		*token = NULL;
	if (check == 2)
		check = 0;
	return (check);
}

int	ft_strtok(char *prompt, char ***envvar, char **token, size_t *i)
{
	size_t	index;
	size_t	tmp_index;

	index = 0;
	while (ft_iswhitespace(prompt[index]) && prompt[index])
		index++;
	if (!prompt[index])
		return ((*i += index), index);
	if (prompt[index] == '"' || ft_strchr("'><$", prompt[index]))
	{
		tmp_index = whitespace_exception(prompt, &index, envvar, token);
		*i += index;
		return (tmp_index);
	}
	*i += index;
	tmp_index = index;
	while (!ft_iswhitespace(prompt[tmp_index]) && prompt[tmp_index])
		tmp_index++;
	*token = malloc((sizeof(char) * (tmp_index - index)) + 1);
	if (!(*token))
		return (ft_print_error("Error: malloc failed\n"));
	tmp_index = 0;
	while (prompt[index] && !ft_iswhitespace(prompt[index])
		&& (prompt[index] != '"' && !ft_strchr("'><$", prompt[index])))
		(*token)[tmp_index++] = prompt[index++];
	(*token)[tmp_index] = '\0';
	*i += index;
	return (0);
}

static int	init_tokenizer(t_token_node **first_node,
									char ***envvar,
									char **tmp_str,
									char *prompt)
{
	size_t	i;

	i = 0;
	if (prompt_error_checks(prompt))
		return (0);
	if (ft_strtok(prompt, envvar, tmp_str, &i))
		return (0);
	*first_node = create_node(*tmp_str, NULL, NULL);
	return (i);
}

int	handle_next_token(t_token_node *first_node,
						char ***envvar,
						char **tmp_str,
						char *prompt)
{
	size_t	i;
	int		check;

	i = 0;
	while ((ft_iswhitespace(prompt[i]) || prompt[i] == '\n') && prompt[i])
		i++;
	if (!prompt[i])
		return (-3);
	check = ft_strtok(&prompt[i], envvar, tmp_str, &i);
	if (check == 1)
		check = -2;
	if (!tmp_str && !first_node->token)
	{
		first_node = create_node(NULL, NULL, NULL);
		check = -2;
	}
	if (check < 0)
		return (check);
	return (i);
}

t_token_node	*tokenizer(char *prompt, 
							char ***envvar, 
							t_token_node *first_nd, 
							int	flow_check)
{
	t_token_node 	*current_node;
	char			*tmp_str;
	size_t			i;

	if (!(i = init_tokenizer(&first_nd, envvar, &tmp_str, prompt)))
		return (ERROR_NULL);
	current_node = first_nd;
	while (ft_strlen(prompt) > i && prompt[i] && flow_check != -3)
	{
		i += check_if_join_args(envvar, &prompt[i], tmp_str, current_node);
		if (!tmp_str)
			return (ft_print_error_null("Error: malloc failed\n"));
		flow_check = handle_next_token(first_nd, envvar, &tmp_str, &prompt[i]);
		if (flow_check == -2)
			return (free_lexer(first_nd), NULL);
		if (flow_check == -1)
			return (free(first_nd->token), first_nd->token = NULL, first_nd);
		i += flow_check;
		if (tmp_str && flow_check != -3)
			current_node->next = create_node(tmp_str, current_node, NULL);
		if (!current_node->next && tmp_str && flow_check != -3)
			return (NULL);
		if (tmp_str && flow_check != -3)
			current_node = current_node->next;
	}
	return (first_nd);
}
