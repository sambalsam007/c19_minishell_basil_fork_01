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

static int	count_key(char **key, 
							char *prompt,
							int *i,
							int *j)
{
	*key = ft_get_key(&prompt[*i + 1]);
	*j += ft_strlen(*key) + 1;
	*i += ft_strlen(*key);
	if (!prompt[*i])
	{
		free(key);
		return (1) ;
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

static int	count_total_strlen(char *prompt,
								char ***envvar,
								char *key,
								int *len_expanded_var)
{
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (prompt[++i] && prompt[i] != '"')
	{
		if (prompt[i] == '$')
		{
			if ((prompt[i+1] && prompt[i+1] == '?') || !prompt[i + 1] 
					|| ft_iswhitespace(prompt[i + 1]))
				;
			else
			{
				if (count_key(&key, prompt, &i, &j) == 1)
					break;
				if (!key)
					return (-1);
				count_value(key, len_expanded_var, envvar);
			}
		}
	}
	if (!prompt[i])
		return (-1);
	return (i - j);
}


/* static int	count_total_strlen(char *prompt, char ***envvar, char *key, */
/* 		int *len_expanded_var) */
/* { */
/* 	int		i; */
/* 	int		j; */
/* 	char	*value; */
/**/
/* 	i = 1; */
/* 	j = 0; */
/* 	while (prompt[i] && prompt[i] != '"') */
/* 	{ */
/* 		if (prompt[i] == '$') */
/* 		{ */
/* 			if ((prompt[i+1] && prompt[i+1] == '?') || !prompt[i + 1]  */
/* 					|| ft_iswhitespace(prompt[i + 1])) */
/* 				; */
/* 			else  */
/* 			{ */
/* 				key = ft_get_key(&prompt[i + 1]); */
/* 				i += ft_strlen(key); */
/* 				if (!prompt[i]) */
/* 					break ; */
/* 				j += ft_strlen(key) + 1; */
/* 				value = ft_get_value(key, envvar); */
/* 				free(key); */
/* 				if (value) */
/* 					*len_expanded_var += ft_strlen(value); */
/* 			} */
/* 		} */
/* 		i++; */
/* 	} */
/* 	if (!prompt[i]) */
/* 		return (-1); */
/* 	return (i - j); */
/* } */
/**/
int	double_quotes(char *prompt,
					size_t *index,
					char ***envvar,
					char **token_str)
{
	char	*key;
	int		i;
	int		len_expanded_var;
	int		flow;

	i = *index;
	flow = 0;
	len_expanded_var = 0;
	if (!prompt[i])
		return (1);
	key = NULL;
	i = count_total_strlen(&prompt[i], envvar, key, &len_expanded_var);
	if (i == -1)
		return (free(key), ft_printf("Error: parentheses not closed\n"), -1);
	*token_str = malloc((sizeof(char) * (i - *index)) + len_expanded_var + 1);
	if (!token_str)
		return (ft_print_error("Error: malloc failed\n"));
	(*index)++;
	flow = fill_token_expd_vars(&prompt[*index], *token_str, key, envvar);
	if (flow == -1)
		return (ft_print_error("Error: malloc failed\n"));
	*index += flow + 1;
	if (!*token_str[0])
		ft_memcpy(*token_str, "", 2);
	return (0);
}

int	single_quotes(char *prompt,
					size_t *index,
					char **tokenized_string)
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
