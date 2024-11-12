/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_handler.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 11:33:46 by bclaeys           #+#    #+#             */
/*   Updated: 2024/11/11 11:34:24 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	fill_redirect_token(char *prompt, char *tokenized_string, int i)
{
	int	temp_index;
	int	white_space_counter;

	temp_index = 0;
	white_space_counter = 0;
	while (prompt[i])
	{
		if (ft_iswhitespace(prompt[i]))
		{
			white_space_counter++;
			while (ft_iswhitespace(prompt[i]))
				i++;
		}
		if (white_space_counter == 2)
			break ;
		tokenized_string[temp_index] = prompt[i];
		i++;
		temp_index++;
	}
	tokenized_string[temp_index] = '\0';
}

// infinite loop
char	*redirect_handler(char *prompt, int *index)
{
	char	*tokenized_string;
	int		i;

	i = *index;
	if (!prompt[i])
		return (NULL);
	while (prompt[i] && !ft_iswhitespace(prompt[i]))
		i++;
	while (prompt[i] && ft_iswhitespace(prompt[i]))
		i++;
	while (prompt[i] && !ft_iswhitespace(prompt[i]))
		i++;
	tokenized_string = malloc((sizeof(char) * (i - *index)) + 1);
	if (!tokenized_string)
		return (ft_print_error_null("Error: malloc failed\n"));
	fill_redirect_token(prompt, tokenized_string, *index);
	*index = i;
	return (tokenized_string);
}
