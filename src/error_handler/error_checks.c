/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_checks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 13:32:58 by bclaeys           #+#    #+#             */
/*   Updated: 2024/11/04 16:19:55 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	prompt_error_checks(char *prompt)
{
	int	i;

	i = 0;
	while (prompt[i] == ' ' || prompt[i] == '\t')
		i++;
	if (prompt[i] == '|')
	{
		ft_print_error("syntax error: unexpected token '|'\n");
		return (1);
	}
	if (!prompt[i])
		return (1);
	return (0);
}
