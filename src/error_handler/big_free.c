/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   big_free.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 12:35:17 by bclaeys           #+#    #+#             */
/*   Updated: 2024/11/04 17:07:37 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	free_lexer(t_token_node *first_node_lexer)
{
	t_token_node	*current;
	t_token_node	*tmp;

	if (!first_node_lexer)
		return ;
	current = first_node_lexer;
	while (current)
	{
		tmp = current;
		current = current->next;
		// Volgende free loopt soms fout, "attempted double free", waarom?
		// Ik kan de fouten ook niet herhalen als ze voorkomen. 
		if (tmp->token)
			free(tmp->token);
		if (tmp)
			free(tmp);
	}
	first_node_lexer = NULL;
}
void	*free_var_data(t_var_data *var_data)
{
	if (var_data->envvar_dict)
		ft_delete_dict(var_data->envvar_dict);
	if (var_data->first_node_lexer)
		free_lexer(var_data->first_node_lexer);
	if (var_data)
		free(var_data);
	return (NULL);
}