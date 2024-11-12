/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 11:47:45 by bclaeys           #+#    #+#             */
/*   Updated: 2024/11/11 11:35:39 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

char	*whitespace_exception(char *prompt, int *index, char ***envvar)
{
	char	*token;
	char	*key;

	key = NULL;
	if (ft_strchr("><", prompt[*index]))
		token = redirect_handler(prompt, index);
	else if (ft_strchr("'", prompt[*index]))
		token = single_quote_handler(prompt, index);
	else if (ft_strchr("$", prompt[*index]))
	{
		key = ft_get_key(&prompt[*index + 1]);
		if (ft_get_value(key, envvar))
			token = ft_strdup(ft_get_value(key, envvar));
		else if (ft_strchr(" ", prompt[*index])) 
			token = ft_strdup("$");
		else 
			/* token = ft_strdup("\n"); */
			token = NULL;
		*index += ft_strlen(key) + 1;
		free(key);
	}
	else if (prompt[*index] == '"')
		token = double_quote_handler(prompt, index, envvar);
	else
		token = NULL;
	return (token);
}

//chechen of eerste command een command is of een redirect: hoe?
static int	type_giver(char *prompt, t_token_node *prev_node)
{
	int				i;
	t_token_node	*temp_node;

	i = 0;
	temp_node = prev_node;
	if (!prompt)
		return (-1);
	if (prompt[i] == '|')
		return (PIPE);
	if (prompt[i] == '-' && prompt[i + 1] == 'n')
		return (FLAG);
	if (prompt[i] == '<' || prompt[i] == '>')
		return (REDIRECT);
	// volgende aanpassen: alles tot pipe of redirect is een argument
	if ((prev_node && prev_node->type == EXEC) ||
		prompt[i] == 39 || prompt[i] == 34)
		return (ARGUMENT);
	while (temp_node && temp_node->type != PIPE)
	{
		if (temp_node->type == EXEC)
			return (ARGUMENT);
		temp_node = temp_node->prev;
	}
	return (EXEC);
}

// c'a'"t" of c'$VALUE'"$VAL" moet ook werken, strings worden
// samengevoegd tot command. Gewoon met de bestaande functies uitpakkan
// en samenvoegen tot node met EXEC type.
int	ft_strtok(char *prompt, char ***envvar, char **token)
{
	int	index;
	int	temp_index;

	index = 0;
	while (ft_iswhitespace(prompt[index]) && prompt[index] && prompt[index])
		index++;
	if (!prompt[index])
		return (index);
	if (prompt[index] == '"' || ft_strchr("'><$", prompt[index]))
	{
		*token = whitespace_exception(prompt, &index, envvar);
		return (index);
	}
	temp_index = index;
	while (!ft_iswhitespace(prompt[temp_index]) && prompt[temp_index])
		temp_index++;
	*token = malloc((sizeof(char) * (temp_index - index)) + 1);
	if (!(*token))
		return (ft_print_error("Error: malloc failed\n"));
	temp_index = 0;
	while (prompt[index] && !ft_iswhitespace(prompt[index])
				&& (prompt[index] != '"' && !ft_strchr("'><$", prompt[index])))
		(*token)[temp_index++] = prompt[index++];
	(*token)[temp_index] = '\0';
	return (index);
}

static t_token_node	*create_node(char *tokenized_str, t_token_node *prev_node,
		t_token_node *current_node)
{
	t_token_node	*token_node;

	if (!current_node)
	{
		token_node = malloc(sizeof(t_token_node));
		if (!token_node)
			return (ft_print_error_null("Error: malloc failed\n"));
		token_node->type = 0;
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
	else if (prev_node != current_node)
		token_node->prev = NULL;
	token_node->next = NULL;
	return (token_node);
}

// echo $PAT$ moet $ printen, $ moet ook $ printen
// gewoon spatie meegeven segfault 
t_token_node	*tokenizer(char *prompt, char ***envvar)
{
	t_token_node	*first_node;
	t_token_node	*current_node;
	char			*temp_str;
	char			*concatenated_str;
	size_t			i;

	i = 0;
	if (prompt_error_checks(prompt))
		return (ERROR_NULL);
	i = ft_strtok(prompt, envvar, &temp_str);
	first_node = create_node(temp_str, NULL, NULL);
	current_node = first_node;
	while (ft_strlen(prompt) > i && prompt[i])
	{
		if (!ft_iswhitespace(prompt[i]) && !ft_strchr("><$", prompt[i]))
		{
			i += ft_strtok(&prompt[i], envvar, &concatenated_str);
			ft_strlcat(temp_str, concatenated_str, ft_strlen(temp_str) 
					+ ft_strlen(concatenated_str) + 1);
			current_node = create_node(temp_str, current_node, current_node);
		}
		else
			i += ft_strtok(&prompt[i], envvar, &temp_str);
		if (!temp_str)
		{
			if (!first_node->token)
			{
				free_lexer(first_node);
				return (create_node(NULL, NULL, NULL));
			}
			continue;
		}
		else
			current_node->next = create_node(temp_str, current_node, NULL);
		if (!current_node->next)
			return (NULL);
		current_node = current_node->next;
	}
	return (first_node);
}
