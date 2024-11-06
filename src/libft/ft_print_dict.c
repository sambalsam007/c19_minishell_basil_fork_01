/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_dict.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 14:11:44 by bclaeys           #+#    #+#             */
/*   Updated: 2024/11/06 14:15:39 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_print_dict(char ***dict, char *separator)
{
	int	i_entry;

	i_entry = 0;
	if (!separator)
		separator = ": ";
	if (!dict)
		return ;
	while (dict[i_entry])
	{
		ft_printf("%s", dict[i_entry][0]);
		ft_printf("%s", separator);
		ft_printf("%s", dict[i_entry][1]);
		ft_printf("\n");
		i_entry++;
	}
	return ;
}
