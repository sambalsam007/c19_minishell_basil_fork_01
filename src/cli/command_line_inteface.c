/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_line_inteface.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 15:47:45 by bclaeys           #+#    #+#             */
/*   Updated: 2024/11/14 19:13:42 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ms_command_line_inteface(t_var_data *var_data)
{
	char			*prompt;
	t_token_node	*tmp;
	int				i;

	prompt = NULL;
	while (true)
	{
		free(prompt);
		prompt = readline("\033[33mbazzels_minishell> \033[0m");
		if (!prompt)
			break ;
		i = 0;
		while (prompt[i] && ft_iswhitespace(prompt[i]))
			i++;
		if (!prompt[0] || !prompt[i])
			continue ;
		if (var_data->first_node_lexer)
			free_lexer(var_data->first_node_lexer);
		var_data->first_node_lexer = tokenizer(prompt, var_data->envvar_dict,
				var_data->first_node_lexer, 0);
		if (!var_data->first_node_lexer)
		{
			free(prompt);
			return (1);
		}
		if (!var_data->first_node_lexer->token
			&& !var_data->first_node_lexer->next)
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
