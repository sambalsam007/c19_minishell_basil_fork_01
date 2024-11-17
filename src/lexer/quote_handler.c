/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 11:13:39 by bclaeys           #+#    #+#             */
/*   Updated: 2024/11/15 15:25:07 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

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
int	double_quotes(char *prompt, size_t *index, char ***envvar, char **token_str)
{
	char	*key;
	int		i;
	int		len_expanded_var;

	i = *index;
	len_expanded_var = 0;
	if (!prompt[i])
		return (1);
	key = NULL;
	i = count_total_strlen(&prompt[i], envvar, key, &len_expanded_var);
	if (i == -1)
		return (free(key), ft_printf("Error: parentheses not closed\n"), -1);
	*token_str = malloc((sizeof(char) * (i - *index)) + len_expanded_var + 10);
	if (!token_str)
		return (ft_print_error("Error: malloc failed\n"));
	(*index)++;
	*index += fill_token_expd_vars(&prompt[*index], *token_str, key, envvar)
		+ 1;
	if (!*token_str[0])
		ft_memcpy(*token_str, "\n", 2);
	return (0);
}

int	single_quotes(char *prompt, size_t *index, char **tokenized_string)
{
	int	i;
	int	j;

	i = *index;
	j = 0;
	if (!prompt[i])
		return (1);
	i++;
	while (prompt[i] && prompt[i] != 39)
		i++;
	if (!prompt[i])
		return (ft_printf("Error: parentheses not closed\n"), -1);
	*tokenized_string = malloc((sizeof(char) * (i - *index)) + 1);
	if (!*tokenized_string)
		return (ft_print_error("Error: malloc failed\n"));
	i = *index + 1;
	while (prompt[i] != 39 && prompt[i])
		(*tokenized_string)[j++] = prompt[i++];
	*index = i + 1;
	(*tokenized_string)[j] = '\0';
	return (0);
}
