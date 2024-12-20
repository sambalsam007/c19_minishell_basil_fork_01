/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 11:47:45 by bclaeys           #+#    #+#             */
/*   Updated: 2024/12/20 17:19:54 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <time.h>

int	T_tokenizer 		= 0;
int	T_init_tokenizer 	= 0;

int	exceptions(char *prompt,
							size_t *index,
							t_var_data *var_data,
							char **token)
{
	int	error_flow;

	error_flow = 0;
	if (ft_strchr("><", prompt[*index]))
	{
		*token = redirect_handler(prompt, index, var_data);
		if (!*token)
			return (-1);
	}
	else if (ft_strchr("'", prompt[*index]))
		error_flow = single_quotes(prompt, index, token);
	else if (ft_strchr("$", prompt[*index]))
		while (prompt[*index] && ft_strchr("$", prompt[*index]) 
				&& error_flow != 2
			&& error_flow != 1)
			error_flow = no_quotes_arg(prompt, index, var_data->envvar, token);
	else if (prompt[*index] == '"')
		error_flow = double_quotes(prompt, index, var_data, token);
	else
		*token = NULL;
	if (error_flow == 2)
		error_flow = 0;
	return (error_flow);
}

int	ft_strtok(char *prompt, t_var_data *var_data, char **token, size_t *i)
{
	size_t	local_index;
	size_t	tmp_index;

	local_index = 0;
	while (ft_iswhitespace(prompt[local_index]) && prompt[local_index])
		local_index++;
	if (!prompt[local_index])
		return ((*i += local_index), local_index);
	if (prompt[local_index] && prompt[local_index] == '|')
		return(*i += 1, *token = ft_strdup("|"), 0);
	if (ft_strchr("'><$\"", prompt[local_index]))
		return (tmp_index = exceptions(prompt, &local_index, 
					var_data, token), *i += local_index, tmp_index);
	tmp_index = local_index;
	while (!ft_iswhitespace(prompt[tmp_index]) && prompt[tmp_index])
		tmp_index++;
	*token = malloc((sizeof(char) * (tmp_index - local_index)) + 1);
	if (!(*token))
		return (ft_print_error("Error: strtok malloc failed\n"));
	tmp_index = 0;
	while (prompt[local_index] && !ft_iswhitespace(prompt[local_index])
		&& (prompt[local_index] != '"' && !ft_strchr("|'><$", prompt[local_index])))
		(*token)[tmp_index++] = prompt[local_index++];
	(*token)[tmp_index] = '\0';
	*i += local_index;
	return (0);
}

static int	init_tokenizer(t_token_node **first_node,
							t_var_data *var_data,
							char **tmp_str,
							char *prompt)
{
	size_t	i;
	int		error_flow;

	i = 0;
	error_flow = ft_strtok(prompt, var_data, tmp_str, &i); if (error_flow == 1) 
	{
		(T_init_tokenizer) ? ft_printf("\t\terror_flow = 1\n"):0;
		return (0);
	}
	if (error_flow == -1)
	{
		(T_init_tokenizer) ? ft_printf("\t\terror_flow = -1\n"):0;
		return (var_data->error_checks->lexer_level_syntax_error = true, -1);
	}
	if (error_flow != 0)
	{
		(T_init_tokenizer) ? ft_printf("\t\terror_flow != 0\n"):0;
		return (error_flow);
	}
	if (!*tmp_str)
	{
		(T_init_tokenizer) ? ft_printf("\t\t!*tmp_str\n"):0;
		*tmp_str = ft_strdup("");
	}
	*first_node = create_node(*tmp_str, NULL, NULL, var_data);
	(T_init_tokenizer) ? ft_printf("\t\treturn (%d)\n", i):0;
	return (i);
}

int	make_token(t_token_node *first_node,
				t_var_data *var_data,
				char **tmp_str,
				char *prompt)
{
	size_t	i;
	int		error_flow;

	i = 0;
	if (var_data->error_checks->lexer_level_syntax_error == true)
		return (-1);
	while ((ft_iswhitespace(prompt[i]) || prompt[i] == '\n') && prompt[i])
		i++;
	if (!prompt[i])
		return (-3);
	error_flow = ft_strtok(&prompt[i], var_data, tmp_str, &i);
	if (error_flow == 1)
		error_flow = -2;
	if (!tmp_str && !first_node->token)
	{
		first_node = NULL;
		error_flow = -2;
	}
	if (error_flow == -1)
		return (var_data->error_checks->lexer_level_syntax_error = true, -1);
	if (error_flow < 0)
		return (error_flow);
	return (i);
}

t_token_node	*tokenizer(char *prompt,
						t_var_data *var_data,
						t_token_node *first_nd,
						int error_flow)
{
	t_token_node	*current;
	char			*tmp_str;
	size_t			i;

	// (T_tokenizer) ? ft_printf("\tprompt: %s\n", prompt):0;
	// (T_tokenizer) ? ft_printf("\terror_flow: %d\n", error_flow):0;
	// (T_tokenizer) ? ft_printf("\tfirst_nd: %p\n", first_nd):0;

	(T_init_tokenizer) ? ft_printf("\t(-) --- init_tokenizer\n"):0;
	i = init_tokenizer(&first_nd, var_data, &tmp_str, prompt);
	(T_init_tokenizer) ? ft_printf("\t(%d) --- init_tokenizer\n", i):0;

	if (i == 0)
		return (ERROR_NULL);
	current = first_nd;
	while (ft_strlen(prompt) > i && prompt[i] && error_flow >= 0 && current)
	{ 
		if (prompt[i - 1] != '|')
			i += check_if_join_args(var_data, &prompt[i], tmp_str, current);
		if (!tmp_str)
			return (ft_print_error_null("Error: tokenizer malloc failed\n"));
		error_flow = make_token(first_nd, var_data, &tmp_str, &prompt[i]);
		if (error_flow < 0 || var_data->error_checks->lexer_level_syntax_error == true)
			break ;
		i += error_flow;
		current->next = create_node(tmp_str, current, NULL, var_data);
		(T_tokenizer) ? ft_printf("\t\ttype: (%d), token: %s\n",current->type, current->token):0;
		current = current->next;
	}
	(T_tokenizer) ? ft_printf("\t\ttype: (%d), token: %s\n",current->type, current->token):0;
	if (var_data->error_checks->lexer_level_syntax_error == true)
		return (first_nd);
	if (error_flow == -2 || !current)
		return (NULL);
	// (T_tokenizer) ? ft_printf("\tfirst_nd: %p\n", first_nd):0;
	return (first_nd);
}
