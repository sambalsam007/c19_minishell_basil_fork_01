/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_line_inteface.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 15:47:45 by bclaeys           #+#    #+#             */
/*   Updated: 2024/11/08 17:21:59 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ms_command_line_inteface(t_var_data *var_data)
{
	char			*prompt;
	t_token_node	*tmp;

	prompt = NULL;
	while (true)
	{
		free(prompt);
		prompt = readline("\033[33mminishell>\033[0m");
		if (!prompt)
			break ;
		if (!prompt[0])
			continue ;
		if (var_data->first_node_lexer)
			free_lexer(var_data->first_node_lexer);
		var_data->first_node_lexer = tokenizer(prompt, var_data->envvar_dict);
		if (!var_data->first_node_lexer->token)
			continue ;
		if (!var_data->first_node_lexer)
			return (1);
		if (!ft_strncmp(prompt, "exit", 5))
			break ;
		tmp = var_data->first_node_lexer;
		while (tmp)
		{
			ft_printf("token: %s, type: %d\n", tmp->token, tmp->type);
			tmp = tmp->next;
		}
	}
	free(prompt);
	return (0);
}
