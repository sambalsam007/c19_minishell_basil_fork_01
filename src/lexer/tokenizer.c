/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 11:47:45 by bclaeys           #+#    #+#             */
/*   Updated: 2024/11/11 11:35:39 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char			*whitespace_exception(char *prompt, int *index, char ***envvar)
{
	char	*return_value;

	if (ft_strchr("><",  prompt[*index]))
		return_value = redirect_handler(prompt, index);
	else if (ft_strchr("'",  prompt[*index]))
		return_value = single_quote_handler(prompt, index);
	else if (prompt[*index] == '"')
		return_value = double_quote_handler(prompt, index, envvar);
	else 
		return_value = NULL;
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

// c'a'"t" of c'$VALUE'"$VAL" moet ook werken, strings worden 
// samengevoegd tot command. Gewoon met de bestaande functies uitpakkan 
// en samenvoegen tot node met EXEC type.
int			ft_strtok(char *prompt, char ***envvar, char **token)
{
	int			index;
	int			temp_index;
	
	index = 0;
	while (ft_iswhitespace(prompt[index]) && prompt[index])
			index++;
	if (!prompt[index])
		return (index);
	if (prompt[index] == '"' || ft_strchr("'><",  prompt[index]))
	{
		*token = whitespace_exception(prompt, &index, envvar);
		return (index);
	}
	temp_index = index;
	while (!ft_iswhitespace(prompt[index]) && prompt[index])
		index++;
	*token = malloc((sizeof(char) * (index - temp_index)) + 1);
	if (!(*token)) 
		return (ft_print_error("Error: malloc failed\n"));	
	index = temp_index;
	temp_index = 0;
	while (prompt[index] && !ft_iswhitespace(prompt[index]))
	{
		(*token)[temp_index] = prompt[index];
		temp_index++;
		index++;
	}
	(*token)[temp_index] = '\0'; 
	return (index);
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
	int				i;

	i = 0;
	if (prompt_error_checks(prompt))
		return (ERROR_NULL);
	i = ft_strtok(prompt, envvar, &temp_str);
	first_node = create_node(temp_str, NULL);
	current_node = first_node;
	while (prompt[i])
	{
		i += ft_strtok(&prompt[i], envvar, &temp_str);
		current_node->next = create_node(temp_str, current_node);
		if (!current_node->next)
			return (NULL);
		current_node = current_node->next;
	}
	// als strtok == NULL, protection toevoegen incl alles freeen
	current_node->next = NULL;
	return (first_node);
}
