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

static int filename_fill_logic(char *prompt,
								int *temp_index,
								char *tokenized_string,
								t_var_data *var_data)
{
	int	i;	
	char *key;

	i = 0;
	key = NULL;
	if (ft_strchr("'\"", prompt[i]))
	{
		fill_token_expd_vars(&prompt[i+1], &tokenized_string[*temp_index], 
				key, var_data->envvar);
		i += count_total_strlen(&prompt[i], var_data, key, temp_index);
		*temp_index += ft_strlen(&tokenized_string[*temp_index]);
		free(key);
	}
	else
		tokenized_string[*temp_index] = prompt[i];
	return (i);
}

static int	fill_redirect_token(char *prompt, 
								char *token,
								int white_space_counter,
								t_var_data *var_data)
{
	int	tmpindex;
	int i;

	tmpindex = 0;
	i = 0;
	while ((size_t)i < ft_strlen(prompt) && prompt[i] && prompt[i] != '|')
	{

		if (ft_iswhitespace(prompt[i]))
		{
			white_space_counter++;
			if (white_space_counter == 2)
				break ;
			while (prompt[i] && ft_iswhitespace(prompt[i]) && token[tmpindex])
				token[tmpindex++] = prompt[i++];
			if (prompt[i] && ft_strchr("<>|", prompt[i]))
				return (var_data->error_checks->lexer_level_syntax_error = true, -1);
		}
		i += filename_fill_logic(&prompt[i], &tmpindex, token, var_data);
		if ((ft_strchr("<>|", prompt[i++]) && white_space_counter >= 1) 
				|| (i > 2 && ft_iswhitespace(prompt[i]) && token[tmpindex++]))
				break;
		tmpindex++;
	}
	return (token[tmpindex] = '\0', 0);
}

static int	filename_length_count_logic(char *prompt,
										size_t *index,
										int *i,
										t_var_data *var_data)
{
	int		len_expanded_var;

	len_expanded_var = 0;
	if (prompt[*i] && *i <= (int)*index + 2)
	{
		while (prompt[*i] && ft_iswhitespace(prompt[*i]) && prompt[*i] != '|')
			(*i)++;
		if (!prompt[*i] || prompt[*i] == '|')
			return (-1);
	}
	else
		return (-1);
	while (prompt[*i] && !ft_iswhitespace(prompt[*i]) 
			&& !ft_strchr("<>", prompt[*i]) && prompt[*i] != '|')
	{
		if (prompt[*i] == '"')
		{
			count_total_strlen(&prompt[(*i)++], var_data, NULL, &len_expanded_var);
			while (prompt[*i] && prompt[*i] != '"')
				(*i)++;
		}
		else
			(*i)++;
	}
	return (len_expanded_var);
}

char	*redirect_handler(char *prompt, size_t *index, t_var_data *var_data)
{
	char	*token_string;
	int		len_expanded_var;
	int		i;

	i = (int)*index;
	len_expanded_var = 0;
	if (!prompt[i])
		return (NULL);
	while (prompt[i] && ft_strchr("><", prompt[i]))
		i++;
	len_expanded_var = filename_length_count_logic(prompt, index, &i, var_data);
	if (len_expanded_var == -1 
			|| var_data->error_checks->lexer_level_syntax_error == true)
		return (ft_printf("Error: redirection syntax error\n"),  ERROR_NULL);	
	token_string = malloc((sizeof(char) * ((i + len_expanded_var) + 1))); // work in progress;
	if (!token_string)
		return (ft_print_error_null("Error: redirect_handler malloc failed\n"));
	fill_redirect_token(&prompt[*index], token_string, 0, var_data);
	check_valid_redir_syntax(token_string, var_data);
	*index += (size_t)i;
	if (var_data->error_checks->lexer_level_syntax_error == true)
		return (ft_printf("Error: redirection syntax error\n"), free(token_string), ERROR_NULL);	
	return (token_string);
}
