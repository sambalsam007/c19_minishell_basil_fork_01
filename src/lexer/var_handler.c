/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 15:19:25 by bclaeys           #+#    #+#             */
/*   Updated: 2024/11/15 18:06:35 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include <stddef.h>
#include <string.h>

char	*ft_get_key(char *prompt)
{
	char	*key;
	int		i;

	i = 0;
	if (!prompt[i])
		return (NULL);
	if (prompt[i] && prompt[i] == '$')
		i++;
	while (prompt[i] && !ft_iswhitespace(prompt[i]) && prompt[i] != '"'
		&& prompt[i] != '$' && prompt[i] != '|')
		i++;
	key = ft_substr(prompt, 0, i);
	return (key);
}

static int	handle_envvars(char *key,
							size_t *j,
							char *token_string,
							char ***envvar)
{
	char	*value;
	size_t	k;

	k = 0;
	if (!key)
		return (1);
	value = ft_get_value(key, envvar);
	while (value && value[k])
		token_string[(*j)++] = value[k++];
	return (0);
}

int	fill_token_expd_vars(char *prmpt,
							char *token_string,
							char *key,
							char ***envvar)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (i < ft_strlen(prmpt) && prmpt[i] && prmpt[i] != '"')
	{
		if (prmpt[i] == '$')
		{
			if ((prmpt[i + 1] && prmpt[i + 1] == '?') || !prmpt[i + 1]
				|| ft_iswhitespace(prmpt[i + 1]) || (prmpt[i + 1]) == '"')
				token_string[j++] = prmpt[i++];
			else
			{
				key = ft_get_key(&prmpt[i + 1]);
				if (handle_envvars(key, &j, token_string, envvar))
					return (-1);
				i += ft_strlen(key) + 1;
				free(key);
			}
		}
		else
			token_string[j++] = prmpt[i++];
	}
	return (token_string[j] = '\0', i);
}

int	check_single_dollar(char *prompt, size_t *index, char **token)
{
	if (!prompt[*index + 1] || ft_iswhitespace(prompt[*index + 1])
		|| prompt[*index + 1] == '"')
	{
		*token = ft_strdup("$");
		*index += 1;
		if (!*token)
			return (1);
	}
	else if (prompt[*index + 1] && prompt[*index + 1] == '?')
	{
		*token = ft_strdup("$?");
		*index += 2;
		if (!*token)
			return (1);
	}
	else
		return (0);
	return (2);
}

int	no_quotes_arg(char *prompt, size_t *index, char ***envvar, char **token)
{
	char	*key;
	int		flow_check;

	key = NULL;
	flow_check = check_single_dollar(prompt, index, token);
	if (flow_check)
		return (flow_check);
	else
	{
		key = ft_get_key(&prompt[*index + 1]);
		if (!key)
			return (1);
		if (ft_get_value(key, envvar))
		{
			*token = ft_strdup(ft_get_value(key, envvar));
			if (!*token)
				return (1);
			return (*index += ft_strlen(key) + 1, free(key), 2);
		}
		else
			*token = ft_strdup("");
		*index += ft_strlen(key) + 1;
		free(key);
	}
	return (0);
}
