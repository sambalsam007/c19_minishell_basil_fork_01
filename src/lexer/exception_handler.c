/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exception_handler.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 11:13:39 by bclaeys           #+#    #+#             */
/*   Updated: 2024/11/05 12:03:40 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"


static char	*get_key(char *prompt)
{
	char **words;
	char *key;

	words = ft_split(prompt, ' ');
	key = words[0]; 
	free(words);
	return (key);
}


static void	expand_variables(char *prompt, char *token_string, char *value)
{
	int		i;
	int		k;
	int		j;

	i = 0;
	j = 0;
	k = 0;
	while (prompt[i] != '"' && prompt[i])
	{
		if (prompt[i] == '$')
		{
			while (value[k])
			{
				token_string[j] = value[k];
				j++;
				k++;
			}
			i += ft_strlen(value);
		}
		else
		{
			token_string[j] = prompt[i];
			i++;
			j++;
		}
	}
	token_string[j] = '\0';
}

char	*double_quote_handler(char *prompt, int *index,  char ***envvar)
{
	char	*token_string;
	char	*key;
	char	*value;
	char	quote;
	int		i;

	i = *index;
	quote = prompt[i];
	if (!prompt[i])
		return (NULL);
	i++;
	while (prompt[i] && prompt[i] != quote)
	{
		if (prompt[i] == '$')
			key = get_key(&prompt[i]);
		i++;
	}
	value = ft_get_value(key, envvar);
	token_string = malloc((sizeof(char) * (i - *index)) + ft_strlen(value) + 1);
	if (!token_string) 
		return (ft_print_error_null("Error: malloc failed\n"));	
	i = *index + 1;
	expand_variables(&prompt[i], token_string, value);
	*index = i;
	free(key);
	return (token_string);
}

char	*single_quote_handler(char *prompt, int *index)
{
	char	*tokenized_string;
	char	quote;
	int		i;
	int		j;

	i = *index;
	j = 0;
	quote = prompt[i];
	if (!prompt[i])
		return (NULL);
	i++;
	while (prompt[i] && prompt[i] != quote)
		i++;
	tokenized_string = malloc((sizeof(char) * (i - *index)) + 1);
	if (!tokenized_string) 
		return (ft_print_error_null("Error: malloc failed\n"));	
	i = *index + 1;
	while (prompt[i] != quote && prompt[i])
	{
		tokenized_string[j] = prompt[i];
		i++;
		j++;
	}
	*index = i;
	tokenized_string[j] = '\0';
	return (tokenized_string);
}

static void	fill_redirect_token(char *prompt, char *tokenized_string, int i)
{
	int	temp_index;
	int	white_space_counter;

	temp_index = 0;
	white_space_counter = 0;
	while (white_space_counter != 2)
	{
		while (prompt[i] && !ft_iswhitespace(prompt[i]))
		{
			tokenized_string[temp_index] = prompt[i];
			i++;
			temp_index++;
		}
		white_space_counter++;
		while (ft_iswhitespace(prompt[i]) && prompt[i])
				i++;
	}
	tokenized_string[temp_index] = '\0';
}

char		*redirect_handler(char *prompt, int *index)
{
	char	*tokenized_string;
	int		i;
	int		temp_index;

	i = *index;
	temp_index = 0;
	if (!prompt[i])
		return (NULL);
	while (temp_index != 2)
	{
		while (prompt[i] && !ft_iswhitespace(prompt[i]))
				i++;
		temp_index++;
	}
	/* ft_printf("%d\n", fff */
	tokenized_string = malloc((sizeof(char) * (i - *index)) + 1);
	if (!tokenized_string) 
		return (ft_print_error_null("Error: malloc failed\n"));	
	temp_index = 0;
	fill_redirect_token(prompt, tokenized_string, *index + 1);
	*index = i;
	return (tokenized_string);
}


