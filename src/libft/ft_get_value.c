/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getentry.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 14:08:55 by bclaeys           #+#    #+#             */
/*   Updated: 2024/11/06 14:11:23 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_get_value(char *key, char ***dict)
{
	int	i_entry;

	i_entry = 0;
	while (dict[i_entry])
	{
		if (dict[i_entry][0] == key)
			return (dict[i_entry][1]);
		i_entry++;
	}
	return (NULL);
}
