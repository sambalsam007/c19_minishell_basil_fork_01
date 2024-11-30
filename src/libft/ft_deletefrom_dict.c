/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_deletefrom_dict.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 14:03:57 by bclaeys           #+#    #+#             */
/*   Updated: 2024/11/29 16:37:06 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	**ft_add_entry(int i_entry,
							int	i_entry_new,
							char ***new_dict, 
							char ***dict)
{
	new_dict[i_entry_new] = malloc(sizeof(char *) * 3);
	if (!new_dict[i_entry_new])
		return (NULL);
	new_dict[i_entry_new][0] = ft_strdup(dict[i_entry][0]);
	new_dict[i_entry_new][1] = ft_strdup(dict[i_entry][1]);
	new_dict[i_entry_new][2] = NULL;
	return (new_dict[i_entry_new]);
}

char	***ft_delete_from_dict(char *key, char ***dict)
{
	int		i_entry;
	int		i_entry_new;
	char	***new_dict;

	i_entry = 0;
	while (dict[i_entry])
		i_entry++;
	if (i_entry == 1)
		return (ft_delete_dict(dict), NULL);
	new_dict = malloc((i_entry) * (sizeof(char **)));
	i_entry = 0;
	i_entry_new = 0;
	while (dict[i_entry])
	{
		if (ft_strncmp(dict[i_entry][0], key, ft_strlen(key)))
		{
			if (!ft_add_entry(i_entry, i_entry_new, new_dict, dict))
					return (NULL);
			i_entry_new++;
		}
		i_entry++;
	}
	new_dict[i_entry_new] = NULL;
	if (i_entry == i_entry_new)
		return (ft_delete_dict(new_dict), dict);
	return (ft_delete_dict(dict), new_dict);
}
