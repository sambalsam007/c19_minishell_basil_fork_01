/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_deletefrom_dict.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 14:03:57 by bclaeys           #+#    #+#             */
/*   Updated: 2024/11/06 14:26:00 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	***ft_delete_from_dict(char *key, char ***dict)
{
	int		i_entry;
	int		i_entry_new;
	char	***new_dict;

	new_dict = malloc(sizeof(dict) - (sizeof(char **) * 2));
	i_entry = 0;
	i_entry_new = 0;
	while (dict[i_entry])
	{
		if (new_dict[i_entry_new][0] == key)
			i_entry++;
		else
		{
			new_dict[i_entry_new][0] = ft_strdup(dict[i_entry][0]);
			new_dict[i_entry_new][1] = ft_strdup(dict[i_entry][1]);
			i_entry++;
			i_entry_new++;
		}
	}
	new_dict[i_entry_new + 1] = NULL;
	ft_delete_dict(dict);
	return (new_dict);
}
