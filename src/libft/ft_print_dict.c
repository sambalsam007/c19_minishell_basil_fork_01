/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_dict.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 14:11:44 by bclaeys           #+#    #+#             */
/*   Updated: 2024/11/28 13:10:49 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_print_dict(char ***dict, char *separator, int fd)
{
	int	i_entry;

	i_entry = 0;
	if (!separator)
		separator = ": ";
	if (!dict)
		return ;
	while (dict[i_entry])
	{
		if (!dict[i_entry][1])
		{
			i_entry++;
			continue ;
		}
		ft_putstr_fd(dict[i_entry][0], fd);
		ft_putstr_fd(separator, fd);
		ft_putstr_fd(dict[i_entry][1], fd);
		ft_putstr_fd("\n", fd);
		i_entry++;
	}
	return ;
}
