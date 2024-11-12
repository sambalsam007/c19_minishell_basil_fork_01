/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exception_handler.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 11:13:39 by bclaeys           #+#    #+#             */
/*   Updated: 2024/11/11 11:35:04 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*ft_get_key(char *prompt)
{
	char	*key;
	int		i;

	i = 0;
	while (prompt[i] && !ft_iswhitespace(prompt[i]) && prompt[i] != '"')
		i++;
	key = ft_substr(prompt, 0, i);
	return (key);
}

static int	fill_token_expd_vars(char *prompt, char *token_string, char *key,
		char ***envvar)
{
	char	*value;
	int		i;
	int		j;
	int		k;

	i = 0;
	j = 0;
	k = 0;
	while (prompt[i] && prompt[i] != '"')
	{
		if (prompt[i] == '$')
		{
			key = ft_get_key(&prompt[i + 1]);
			value = ft_get_value(key, envvar);
			while (value && value[k])
				token_string[j++] = value[k++];
			k = 0;
			i += ft_strlen(key) + 1;
			free(key);
		}
		else
			token_string[j++] = prompt[i++];
	}
	token_string[j] = '\0';
	return (i);
}

static int	count_total_strlen(char *prompt, char ***envvar, char *key,
		int *len_expanded_var)
{
	int		i;
	int		j;
	char	*value;

	i = 1;
	j = 0;
	while (prompt[i] && prompt[i] != '"')
	{
		if (prompt[i] == '$')
		{
			key = ft_get_key(&prompt[i + 1]);
			i += ft_strlen(key);
			if (!prompt[i])
				break ;
			j += ft_strlen(key) + 1;
			value = ft_get_value(key, envvar);
			free(key);
			if (value)
				*len_expanded_var += ft_strlen(value);
		}
		i++;
	}
	if (!prompt[i])
		return (-1);
	return (i - j);
}

// a) als ik echo "$PATH" hey "$PAT" | hey ingeef interpreteert het de tweede 
// 'hey' twee keer en crasht het de volgende keer, of ligt dat aan ctrl copy?
char	*double_quote_handler(char *prompt, int *index, char ***envvar)
{
	char	*token_str;
	char	*key;
	int		i;
	int		len_expanded_var;

	i = *index;
	len_expanded_var = 0;
	if (!prompt[i])
		return (NULL);
	key = NULL;
	i = count_total_strlen(&prompt[i], envvar, key, &len_expanded_var);
	if (i == -1)
	{
		free(key);
		return (ft_print_error_null("Error: parentheses not closed\n"));
	}
	token_str = malloc((sizeof(char) * (i - *index)) + len_expanded_var + 10);
	if (!token_str)
		return (ft_print_error_null("Error: malloc failed\n"));
	(*index)++;
	*index += fill_token_expd_vars(&prompt[*index], token_str, key, envvar) + 1;
	if (!token_str[0])
		ft_memcpy(token_str, "\n", 2);
	return (token_str);
}

char	*single_quote_handler(char *prompt, int *index)
{
	char	*tokenized_string;
	int		i;
	int		j;

	i = *index;
	j = 0;
	if (!prompt[i])
		return (NULL);
	i++;
	while (prompt[i] && prompt[i] != 39)
		i++;
	if (!prompt[i])
		return (ft_print_error_null("Error: parentheses not closed\n"));
	tokenized_string = malloc((sizeof(char) * (i - *index)) + 1);
	if (!tokenized_string)
		return (ft_print_error_null("Error: malloc failed\n"));
	i = *index + 1;
	while (prompt[i] != 39 && prompt[i])
		tokenized_string[j++] = prompt[i++];
	*index = i + 1;
	tokenized_string[j] = '\0';
	return (tokenized_string);
}
