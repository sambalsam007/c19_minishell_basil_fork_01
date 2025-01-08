/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_handler.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 11:33:46 by bclaeys           #+#    #+#             */
/*   Updated: 2025/01/07 12:53:20 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include <stddef.h>

static int	check_valid_redir_syntax(char *tokenized_string,
									t_var_data *var_data)
{
	char	**redir_split;
	int		i;

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
	if (redir_split[0][1] && (redir_split[0][1] != '>'
			&& redir_split[0][1] != '<'))
		var_data->error_checks->lexer_level_syntax_error = true;
	if (redir_split[1][0] == '>' || redir_split[1][0] == '<')
		var_data->error_checks->lexer_level_syntax_error = true;
	return (free(redir_split[0]), free(redir_split[1]), free(redir_split), 0);
}

static int	filename_length_count_logic(char *prompt,
										size_t *index,
										int *i,
										t_var_data *var_data)
{
	int	len_exp_var;

	len_exp_var = 0;
	if (prompt[*i] && *i <= (int)*index + 2)
	{
		while (prompt[*i] && ft_iswhitespace(prompt[*i]) && prompt[*i] != '|')
			(*i)++;
		if (!prompt[*i] || prompt[*i] == '|')
			return (-1);
	}
	else
		return (-1);
	while (prompt[*i] && !ft_iswhitespace(prompt[*i]) && !ft_strchr("<>",
			prompt[*i]) && prompt[*i] != '|')
	{
		if (prompt[*i] == '"')
		{
			count_total_strlen(&prompt[(*i)++], var_data, NULL, &len_exp_var);
			while (prompt[*i] && prompt[*i] != '"')
				(*i)++;
		}
		else
			(*i)++;
	}
	return (len_exp_var);
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
		return (ft_printf_fd(2, "Err: redirection syntax error\n"), ERROR_NULL);
	token_string = malloc((sizeof(char) * ((i + len_expanded_var) + 2)));
	if (!token_string)
		return (ft_printf_fd(2, "Err: redirect_handler malloc\n"), ERROR_NULL);
	fill_redirect_token(&prompt[*index], token_string, 0, var_data);
	check_valid_redir_syntax(token_string, var_data);
	*index += (size_t)i;
	if (var_data->error_checks->lexer_level_syntax_error == true)
		return (ft_printf_fd(2, "Err: redirection syntax error\n"),
			free(token_string), ERROR_NULL);
	return (token_string);
}
