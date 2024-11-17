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
#include <string.h>

char	*ft_get_key(char *prompt)
{
	char	*key;
	int		i;

	i = 0;
	if (!prompt[i])
		return (NULL);
	while (prompt[i] && !ft_iswhitespace(prompt[i]) && prompt[i] != '"' 
			&& prompt[i] != '$')
		i++;
	key = ft_substr(prompt, 0, i);
	return (key);
}

int	fill_token_expd_vars(char *prompt, char *token_string, char *key,
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

int	no_quotes_arg(char *prompt, size_t *index, char ***envvar, char **token)
{
	char	*key;

	key = NULL;
	if (!prompt[*index + 1] || ft_iswhitespace(prompt[*index + 1]))
	{	
		*token = ft_strdup("$");
		*index += 1;
		if (!*token)
			return (1);
	}
	else
	{
		key = ft_get_key(&prompt[*index + 1]);
		if (!key)
			return(1);
		if (ft_get_value(key, envvar))
		{
			*token = ft_strdup(ft_get_value(key, envvar));
			if (!*token)
				return (1);
			*index += ft_strlen(key) + 1;
			return(free(key), 2);
		}
		else
			*token = NULL;
		*index += ft_strlen(key) + 1;
		free(key);
	}
	return (0);
}
