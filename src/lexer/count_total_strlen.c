/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_total_strlen.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 12:56:09 by bclaeys           #+#    #+#             */
/*   Updated: 2025/01/07 12:56:50 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	count_key(char **key,
				char *prompt,
				int *i,
				int *j)
{
	*key = ft_get_key(&prompt[*i + 1]);
	*j += ft_strlen(*key);
	*i += ft_strlen(*key);
	if (!prompt[*i])
	{
		free(key);
		return (1);
	}
	return (0);
}

static int	count_value(char *key,
						int *len_expanded_var,
						char ***envvar)
{
	char	*value;

	value = ft_get_value(key, envvar);
	free(key);
	if (value)
		*len_expanded_var += ft_strlen(value);
	return (0);
}

int	count_total_strlen(char *prompt,
						t_var_data *var_data,
						char *key,
						int *len_expanded_var)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (prompt[++i] && prompt[i] != '"' && prompt[i] != '\'')
	{
		if (prompt[i] == '$')
		{
			if ((prompt[i + 1] && prompt[i + 1] == '?') || !prompt[i + 1]
				|| ft_iswhitespace(prompt[i + 1]))
				;
			else
			{
				if (count_key(&key, prompt, &i, &j) == 1)
					break ;
				if (!key)
					return (-1);
				count_value(key, len_expanded_var, var_data->envvar);
			}
		}
	}
	if (!prompt[i] && prompt[i - 1] != '"')
		return (var_data->error_checks->lexer_level_syntax_error = true, -1);
	return (i - j);
}
