/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_handler.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 11:33:46 by bclaeys           #+#    #+#             */
/*   Updated: 2024/11/14 17:15:44 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include <stddef.h>

static int	check_valid_redir_syntax(char *tokenized_string, 
								t_var_data *var_data)
{
	char **redir_split;
	int	i;

	i = 0;
	while (tokenized_string[i] && !ft_strchr("><", tokenized_string[i]))
		i++;
	if (i > 2)
		return (var_data->error_checks->lexer_level_syntax_error = true, 1);
	if (tokenized_string[i] && !ft_iswhitespace(tokenized_string[i]))
		return (0);
	while (tokenized_string[i] && ft_iswhitespace(tokenized_string[i]))
		i++;
	if (!tokenized_string[i]) 
		return (var_data->error_checks->lexer_level_syntax_error = true, 1);
	redir_split = ft_split(tokenized_string, ' ');
	if (ft_strlen(redir_split[0]) > 2)
		return (var_data->error_checks->lexer_level_syntax_error = true, 1);
	if (redir_split[0][1] 
			&& (redir_split[0][1] != '>' && redir_split[0][1] != '<'))
		var_data->error_checks->lexer_level_syntax_error = true;
	if (redir_split[1][0] == '>' || redir_split[1][0] == '<') 
		var_data->error_checks->lexer_level_syntax_error = true;
	return (free(redir_split[0]), free(redir_split[1]), free(redir_split), 0);
}

static int	fill_redirect_token(char *prompt, 
								char *tokenized_string, 
								int i,
								t_error_checks *error_checks)
{
	int	temp_index;
	int	white_space_counter;

	temp_index = 0;
	white_space_counter = 0;
	while (prompt[i])
	{
		if (ft_iswhitespace(prompt[i]))
			white_space_counter++;
		if (white_space_counter == 2)
			break ;
		if (ft_iswhitespace(prompt[i]))
		{
			while (ft_iswhitespace(prompt[i]) && tokenized_string[temp_index])
				tokenized_string[temp_index++] = prompt[i++];
			if (prompt[i] && ft_strchr("<>", prompt[i]))
				return (error_checks->lexer_level_syntax_error = true, -1);
		}
		tokenized_string[temp_index] = prompt[i];
		if (white_space_counter >= 1 && ft_strchr("<>", prompt[i]))
				break;
		i++;
		temp_index++;
	}
	return (tokenized_string[temp_index] = '\0', 0);
}

char	*redirect_handler(char *prompt, size_t *index, t_var_data *var_data)
{
	char	*token_string;
	int		i;

	i = (int)*index;
	if (!prompt[i])
		return (NULL);
	while (prompt[i] && !ft_iswhitespace(prompt[i]))
		i++;
	if (prompt[i] && i <= (int)*index + 2)
	{
		while (prompt[i] && ft_iswhitespace(prompt[i]) && prompt[i] != '|')
			i++;
		if (!prompt[i] || prompt[i] == '|')
			return (ft_printf("Error: redirection syntax error\n"), ERROR_NULL);
	}
	else
		return (ft_printf("Error: redirection syntax error\n"), ERROR_NULL);
	while (prompt[i] && !ft_iswhitespace(prompt[i]) 
			&& !ft_strchr("<>", prompt[i]))
		i++;
	token_string = malloc((sizeof(char) * (i - (int)*index)) + 1);
	if (!token_string)
		return (ft_print_error_null("Error: malloc failed\n"));
	fill_redirect_token(prompt, token_string, *index, var_data->error_checks);
	check_valid_redir_syntax(token_string, var_data);
	*index = (size_t)i;
	if (var_data->error_checks->lexer_level_syntax_error == true)
		return (ft_printf("Error: redirection syntax error\n"), free(token_string), ERROR_NULL);	
	return (token_string);
}
