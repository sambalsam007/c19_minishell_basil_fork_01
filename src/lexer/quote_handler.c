/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 11:13:39 by bclaeys           #+#    #+#             */
/*   Updated: 2025/01/07 12:57:29 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include <stddef.h>

static void	init_local_variables(int *i,
								size_t *index,
								int *flow,
								int *len_expanded_var)
{
	*i = *index;
	*flow = 0;
	*len_expanded_var = 0;
}

int	double_quotes(char *prompt,
					size_t *index,
					t_var_data *var_data,
					char **token_str)
{
	char	*key;
	int		i;
	int		len_expanded_var;
	int		flow;

	if (!prompt[0])
		return (1);
	key = NULL;
	init_local_variables(&i, index, &flow, &len_expanded_var);
	i = count_total_strlen(&prompt[i], var_data, key, &len_expanded_var);
	if (i == -1)
		return (free(key), ft_printf_fd(2, "Oops: close ur parentheses\n"), -1);
	*token_str = malloc((sizeof(char) * (i - *index)) + len_expanded_var + 1);
	if (!token_str)
		return (ft_printf_fd(2, "Err: malloc failed\n"), 1);
	(*index)++;
	flow = fill_token_expd_vars(&prompt[*index], *token_str, key,
			var_data->envvar);
	if (flow == -1)
		return (ft_printf_fd(2, "Err: malloc failed\n"), 1);
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
	while (prompt[i] && prompt[i] != '\'')
		i++;
	if (!prompt[i])
		return (ft_printf_fd(2, "Oops: close ur parentheses\n"), -1);
	*tokenized_string = malloc((sizeof(char) * (i - *index)) + 1);
	if (!*tokenized_string)
		return (ft_printf_fd(2, "Err: malloc failed\n"), 1);
	i = *index + 1;
	while (prompt[i] != '\'' && prompt[i])
		(*tokenized_string)[j++] = prompt[i++];
	*index = i + 1;
	(*tokenized_string)[j] = '\0';
	return (0);
}
