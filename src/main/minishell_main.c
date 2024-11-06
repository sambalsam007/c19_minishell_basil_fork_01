/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_main.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 11:48:11 by bclaeys           #+#    #+#             */
/*   Updated: 2024/11/05 12:03:17 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"


int	main(int ac, char **av, char **envp)
{
	t_var_data		*var_data;
	t_token_node	*tmp;

	// volgende weggecomment om te testen, en voidac toegevoegd
	/* if (ac != 1) */
	/* 	return (1); */
	(void)ac;
	var_data = init_var_data(envp);
	if (!var_data)
		return (1);
	var_data->first_node_lexer = tokenizer(av[1], var_data->envvar_dict);
	tmp = var_data->first_node_lexer;
	while (var_data->first_node_lexer->next)
	{
		ft_printf("token: %s, type: %d\n", var_data->first_node_lexer->token, 
				var_data->first_node_lexer->type);
		var_data->first_node_lexer = var_data->first_node_lexer->next;
	}
	var_data->first_node_lexer = tmp;
	free_var_data(var_data);
	(void)av;
}
