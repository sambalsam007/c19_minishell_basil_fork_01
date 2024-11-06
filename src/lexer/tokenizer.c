/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 11:47:45 by bclaeys           #+#    #+#             */
/*   Updated: 2024/11/05 12:03:16 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char			*whitespace_exception(char *prompt, int *index, char ***envvar)
{
	void	*return_value;

	if (!ft_strchr("><",  prompt[*index]))
		return_value = redirect_handler;
	if (!ft_strchr("'",  prompt[*index]))
		return_value = single_quote_handler(prompt, index);
	if (prompt[*index] == '"')
		return_value = double_quote_handler(prompt, index, envvar);
	return (return_value);
}

//chechen of eerste command een command is of een redirect: hoe?
static int			type_giver(char *prompt, t_token_node *prev_node)
{
	int	i;

	i = 0;
	if (prompt[i] == '|')
		return (PIPE);	
	if (prompt[i] == '-' && prompt[i + 1] == 'n')
		return (FLAG);	
	if (prompt[i] == '<' || prompt[i] == '>')
		return (REDIRECT);	
	// volgende aanpassen: alles tot pipe of redirect is een argument
	if ((prev_node && prev_node->type == EXEC) || 
			prompt[i] == 39 || prompt[i] == 34)
		return (ARGUMENT);
	return (EXEC);
}

char 				*ft_strtok(char *prompt, char ***envvar)
{
	static int	index;
	char		*tokenized_string;
	int			temp_index;
	
	while (ft_iswhitespace(prompt[index]) && prompt[index])
			index++;
	if (!prompt[index])
		return (NULL);
	if (ft_strchr("'><",  prompt[index] || prompt[index] == '"'))
		return (whitespace_exception(prompt, &index, envvar));
	temp_index = index ;
	while (!ft_iswhitespace(prompt[index]) && prompt[index])
		index++;
	tokenized_string = malloc((sizeof(char) * (index - temp_index)) + 1);
	if (!tokenized_string) 
		return (ft_print_error_null("Error: malloc failed\n"));	
	index = temp_index;
	temp_index = 0;
	while (prompt[index] && !ft_iswhitespace(prompt[index]))
	{
		tokenized_string[temp_index] = prompt[index];
		temp_index++;
		index++;
	}
	tokenized_string[temp_index] = '\0'; 
	return (tokenized_string);
}

static t_token_node	*create_node(char *tokenized_str, t_token_node *prev_node)
{
	t_token_node *token_node;

	token_node = malloc(sizeof(t_token_node));
	if (!token_node) 
		return (ft_print_error_null("Error: malloc failed\n"));	
	token_node->token = tokenized_str;
	token_node->type = type_giver(tokenized_str, prev_node);
	if (prev_node)
	{
		prev_node->next = token_node;
		token_node->prev = prev_node;
	}
	else
		token_node->prev = NULL;
	return (token_node);
}

t_token_node		*tokenizer(char *prompt, char ***envvar)
{
	t_token_node 	*first_node;
	t_token_node 	*current_node;
	char			*temp_str;

	if (prompt_error_checks(prompt))
		return (ERROR_NULL);
	first_node = create_node(ft_strtok(prompt, envvar), NULL);
	current_node = first_node;
	while ((temp_str = ft_strtok(prompt, envvar)) != NULL)
		current_node = create_node(temp_str, current_node);
	// als strtok == NULL, protection toevoegen incl alles freeen
	current_node->next = NULL;
	return (first_node);
}
