/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_messages.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 17:03:23 by bclaeys           #+#    #+#             */
/*   Updated: 2024/11/04 17:05:20 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_print_error(char *string)
{
	//change ft_printf to take a fd?
	ft_printf(string);
	return (1);
}

void	*ft_print_error_null(char *string)
{
	//change ft_printf to take a fd?
	ft_printf(string);
	return (NULL);
}
