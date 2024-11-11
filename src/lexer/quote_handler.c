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

static char	*get_key(char *prompt)
{
	char 	*key;
	int		i;

	i = 0;
	while (!ft_iswhitespace(prompt[i]) && prompt[i] != '"')
		i++;
	key = ft_substr(prompt, 0, i); 
	return (key);
}


static int	fill_token_expd_vars(char *prompt, char *token_string, 
		char *value, char *key)
{
	int		i;
	int		k;
	int		j;

	i = 0;
	j = 0;
	k = 0;
	while (prompt[i] && prompt[i] != '"' )
	{
		if (prompt[i] == '$')
		{
			while (value && value[k])
				token_string[j++] = value[k++];
			i += ft_strlen(key) + 1;
		}
		else
			token_string[j++] = prompt[i++];
	}
	token_string[j] = '\0';
	return (i);
}

// als ik twee keer $ gebruik in een argument segfault de volgende command, 
// of doet het raar over het algemeen, bv: echo "HEY $PAT ewa $HEHE lol" 
// = logisch, ik handle geen twee vars in een string
char	*double_quote_handler(char *prompt, int *index,  char ***envvar)
{
	char	*token_str;
	char	*key;
	char	*value;
	int		i;
	int		len_expanded_var;

	i = *index;
	len_expanded_var = 0;
	if (!prompt[i])
		return (NULL);
	i++;
	key = NULL;
	while (prompt[i] && prompt[i] != '"')
	{
		if (prompt[i] == '$')
		{
			
			key = get_key(&prompt[i + 1]);
			i += ft_strlen(key) + 1;
			value = ft_get_value(key, envvar);
			if (!value)
				continue;		
			len_expanded_var += ft_strlen(value);
		}
		i++;
	}
	if (key)
		i -= ft_strlen(key);
	token_str = malloc((sizeof(char) * (i - *index)) + len_expanded_var + 1);
	if (!token_str) 
		return (ft_print_error_null("Error: malloc failed\n"));	
	(*index)++;
	*index += fill_token_expd_vars(&prompt[*index], token_str, value, key) + 1;
	if (key)
		free(key);
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
	tokenized_string = malloc((sizeof(char) * (i - *index)) + 1);
	if (!tokenized_string) 
		return (ft_print_error_null("Error: malloc failed\n"));	
	i = *index + 1;
	while (prompt[i] != 39 && prompt[i])
	{
		tokenized_string[j] = prompt[i];
		i++;
		j++;
	}
	*index = i + 1;
	tokenized_string[j] = '\0';
	return (tokenized_string);
}
