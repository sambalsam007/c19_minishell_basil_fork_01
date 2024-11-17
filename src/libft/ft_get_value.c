/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_value.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 14:08:55 by bclaeys           #+#    #+#             */
/*   Updated: 2024/11/15 17:11:33 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_get_value(char *key, char ***dict)
{
	int	i_entry;
	int	longest_len;

	i_entry = 0;
	longest_len = 0;
	while (dict[i_entry])
	{
		if (ft_strlen(key) >= ft_strlen(dict[i_entry][0]))
			longest_len = ft_strlen(key);
		if (ft_strlen(key) < ft_strlen(dict[i_entry][0]))
			longest_len = ft_strlen(dict[i_entry][0]);
		if (!ft_strncmp(dict[i_entry][0], key, longest_len))
			return (dict[i_entry][1]);
		i_entry++;
	}
	return (NULL);
}
