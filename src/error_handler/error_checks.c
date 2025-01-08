/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_checks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 13:32:58 by bclaeys           #+#    #+#             */
/*   Updated: 2024/11/18 19:03:06 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	prompt_error_checks(t_token_node *first_node,
						t_error_checks *error_checks)
{
	if (!first_node)
		return (1);
	if (first_node->type == PIPE)
	{
		error_checks->parser_level_syntax_error = true;
		return (ft_print_error("Error: unexpected token '|'\n"), 1);
	}
	if (first_node->type == FLAG)
	{
		error_checks->parser_level_syntax_error = true;
		return (ft_print_error("Error: unexpected token '-n'\n"), 1);
	}
	return (0);
}
