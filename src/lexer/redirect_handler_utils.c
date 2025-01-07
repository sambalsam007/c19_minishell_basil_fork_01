/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_handler_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 12:50:35 by bclaeys           #+#    #+#             */
/*   Updated: 2025/01/07 12:51:59 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	init_i_and_check_for_redir(int *i, char *prompt, int tmpindex)
{
	*i = 0;
	if (ft_strchr("<>", prompt[*i]) && tmpindex > 2)
		return (1);
	return (0);
}

static int	single_quotes_logic(int *i,
						int *tmpindex,
						char *prompt,
						char *tokenized_string)
{
	(*i)++;
	while (prompt[*i] && prompt[*i] != '\'')
	{
		tokenized_string[(*tmpindex)++] = prompt[(*i)++];
		if (!prompt[*i])
			return (1);
	}
	return (0);
}

static int	filename_fill_logic(char *prompt,
								int *tmpindex,
								char *tokenized_string,
								t_var_data *var_data)
{
	int		i;
	char	*key;

	key = NULL;
	init_i_and_check_for_redir(&i, prompt, *tmpindex);
	if (ft_strchr("\"", prompt[i]))
	{
		fill_token_expd_vars(&prompt[i + 1], &tokenized_string[*tmpindex],
			key, var_data->envvar);
		i += count_total_strlen(&prompt[i], var_data, key, tmpindex);
		*tmpindex += ft_strlen(&tokenized_string[*tmpindex]);
		free(key);
	}
	else if (ft_strchr("'", prompt[i]))
	{
		if (single_quotes_logic(&i, tmpindex, prompt, tokenized_string))
			return (var_data->error_checks->lexer_level_syntax_error = true,
				-1);
	}
	else
		tokenized_string[*tmpindex] = prompt[i];
	if (prompt[0] != '\'')
		(*tmpindex)++;
	return (i);
}

static void	init_i_and_tmpindex_for_token(int *i, int *tmpindex)
{
	*tmpindex = 0;
	*i = 0;
}

int	fill_redirect_token(char *prompt,
								char *token,
								int space_counter,
								t_var_data *var_data)
{
	int	tmpindex;
	int	i;

	init_i_and_tmpindex_for_token(&i, &tmpindex);
	while ((size_t)i < ft_strlen(prompt) && prompt[i] && prompt[i] != '|')
	{
		if (ft_iswhitespace(prompt[i]))
		{
			space_counter++;
			if (space_counter == 2)
				break ;
			while (prompt[i] && ft_iswhitespace(prompt[i]) && token[tmpindex])
				token[tmpindex++] = prompt[i++];
			if (prompt[i] && ft_strchr("<>|", prompt[i]))
				return (var_data->error_checks->lexer_level_syntax_error = true,
					-1);
		}
		i += filename_fill_logic(&prompt[i], &tmpindex, token, var_data);
		if (((ft_strchr("<>|", prompt[i]) && (space_counter >= 1 || i > 3))
				|| (i > 2 && ft_iswhitespace(prompt[i]) && token[tmpindex++])
				|| var_data->error_checks->lexer_level_syntax_error == true))
			break ;
		i++;
	}
	return (token[tmpindex] = '\0', var_data->is_redirect = true, 0);
}
