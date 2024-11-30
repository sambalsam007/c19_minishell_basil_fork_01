/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_update_dict.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 12:43:43 by bclaeys           #+#    #+#             */
/*   Updated: 2024/11/30 13:11:58 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

void	ft_update_dict(char *key, char *new_value, char ***dict)
{
	int		i_entry;
	int		longest_len;
	char 	*str_to_change;

	i_entry = 0;
	longest_len = 0;
	str_to_change = NULL;
	if (!dict)
		return ;
	while (dict[i_entry])
	{
		if (ft_strlen(key) >= ft_strlen(dict[i_entry][0]))
			longest_len = ft_strlen(key);
		if (ft_strlen(key) < ft_strlen(dict[i_entry][0]))
			longest_len = ft_strlen(dict[i_entry][0]);
		if (!ft_strncmp(dict[i_entry][0], key, longest_len))
			break;
		i_entry++;
	}
	str_to_change = dict[i_entry][1];
	if (!str_to_change)
		ft_addto_dict(key, new_value, dict);
	else
		dict[i_entry][1] = new_value; 
	free(str_to_change);
}
