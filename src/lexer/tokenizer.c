/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 11:47:45 by bclaeys           #+#    #+#             */
/*   Updated: 2024/11/18 16:56:03 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <time.h>

int	whitespace_exception(char *prompt,
							size_t *index,
							t_var_data *var_data,
							char **token)
{
	int	check;

	check = 0;
	if (ft_strchr("><", prompt[*index]))
	{
		*token = redirect_handler(prompt, index, var_data);
		if (!*token)
			return (-1);
	}
	else if (ft_strchr("'", prompt[*index]))
		check = single_quotes(prompt, index, token);
	else if (ft_strchr("$", prompt[*index]))
		while (prompt[*index] && ft_strchr("$", prompt[*index]) && check != 2
			&& check != 1)
			check = no_quotes_arg(prompt, index, var_data->envvar, token);
	else if (prompt[*index] == '"')
		check = double_quotes(prompt, index, var_data, token);
	else
		*token = NULL;
	if (check == 2)
		check = 0;
	return (check);
}

int	ft_strtok(char *prompt, t_var_data *var_data, char **token, size_t *i)
{
	size_t	index;
	size_t	tmp_index;

	index = 0;
	while (ft_iswhitespace(prompt[index]) && prompt[index])
		index++;
	if (!prompt[index])
		return ((*i += index), index);
	if (prompt[index] && prompt[index] == '|')
		return(*i += 1, *token = ft_strdup("|"), 0);
	if (prompt[index] == '"' || ft_strchr("'><$", prompt[index]))
		return (tmp_index = whitespace_exception(prompt, &index, 
					var_data, token), *i += index, tmp_index);
	/* *i += index; */
	// waarom was dit nodig?
	tmp_index = index;
	while (!ft_iswhitespace(prompt[tmp_index]) && prompt[tmp_index])
		tmp_index++;
	*token = malloc((sizeof(char) * (tmp_index - index)) + 1);
	if (!(*token))
		return (ft_print_error("Error: strtok malloc failed\n"));
	tmp_index = 0;
	while (prompt[index] && !ft_iswhitespace(prompt[index])
		&& (prompt[index] != '"' && !ft_strchr("|'><$", prompt[index])))
		(*token)[tmp_index++] = prompt[index++];
	(*token)[tmp_index] = '\0';
	*i += index;
	return (0);
}

static int	init_tokenizer(t_token_node **first_node,
							t_var_data *var_data,
							char **tmp_str,
							char *prompt)
{
	size_t	i;
	int		flow;

	i = 0;
	flow = ft_strtok(prompt, var_data, tmp_str, &i);
	if (flow == 1)
		return (0);
	if (flow == -1)
		return (var_data->error_checks->lexer_level_syntax_error = true, -1);
	if (flow != 0)
		return (flow);
	if (!*tmp_str)
		*tmp_str = ft_strdup("");
	*first_node = create_node(*tmp_str, NULL, NULL);
	return (i);
}

int	make_token(t_token_node *first_node,
				t_var_data *var_data,
				char **tmp_str,
				char *prompt)
{
	size_t	i;
	int		check;

	i = 0;
	if (var_data->error_checks->lexer_level_syntax_error == true)
		return (-1);
	while ((ft_iswhitespace(prompt[i]) || prompt[i] == '\n') && prompt[i])
		i++;
	if (!prompt[i])
		return (-3);
	check = ft_strtok(&prompt[i], var_data, tmp_str, &i);
	if (check == 1)
		check = -2;
	if (!tmp_str && !first_node->token)
	{
		first_node = NULL;
		check = -2;
	}
	if (check == -1)
		return (var_data->error_checks->lexer_level_syntax_error = true, -1);
	if (check < 0)
		return (check);
	return (i);
}

t_token_node	*tokenizer(char *prompt,
						t_var_data *var_data,
						t_token_node *first_nd,
						int flow)
{
	t_token_node	*current;
	char			*tmp_str;
	size_t			i;

	i = init_tokenizer(&first_nd, var_data, &tmp_str, prompt);
	if (i == 0)
		return (ERROR_NULL);
	current = first_nd;
	while (ft_strlen(prompt) > i && prompt[i] && flow >= 0 && current)
	{ 
		if (prompt[i-1] != '|')
			i += check_if_join_args(var_data, &prompt[i], tmp_str, current);
		if (!tmp_str)
			return (ft_print_error_null("Error: tokenizer malloc failed\n"));
		flow = make_token(first_nd, var_data, &tmp_str, &prompt[i]);
		if (flow < 0 || var_data->error_checks->lexer_level_syntax_error == true)
			break ;
		i += flow;
		current->next = create_node(tmp_str, current, NULL);
		current = current->next;
	}
	if (var_data->error_checks->lexer_level_syntax_error == true)
		return (first_nd);
	if (flow == -2 || !current)
		return (NULL);
	return (first_nd);
}
