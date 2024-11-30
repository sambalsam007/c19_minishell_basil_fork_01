/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_addto_dict.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 13:27:35 by bclaeys           #+#    #+#             */
/*   Updated: 2024/11/29 16:37:05 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	***ft_entry(int i_entry, char ***new_dict, char ***dict)
{
	new_dict[i_entry] = malloc(sizeof(char *) * 3);
	if (!new_dict[i_entry])
		return (NULL);
	new_dict[i_entry][0] = ft_strdup(dict[i_entry][0]);
	if (dict[i_entry][1])
		new_dict[i_entry][1] = ft_strdup(dict[i_entry][1]);
	else 
		new_dict[i_entry][1] = ft_strdup(" ");
	new_dict[i_entry][2] = NULL;
	return (new_dict);
}

static char	***ft_new_dict(char *key, char *value)
{
	char	***new_dict;

	new_dict = malloc(sizeof(char **));
	if (!new_dict)
	{
		ft_putstr_fd("error: malloc failed", 2);
		return (NULL);
	}
	new_dict[0] = malloc(sizeof(char *) * 3);
	new_dict[0][0] = ft_strdup(key);
	new_dict[0][1] = ft_strdup(value);
	new_dict[0][2] = NULL;
	new_dict[1] = NULL;
	return (new_dict);
}

char	***ft_existing_dict(char *key, char *value, char ***dict)
{
	int		i_entry;
	char	***new_dict;

	i_entry = 0;
	while (dict[i_entry])
		i_entry++;
	new_dict = malloc((i_entry + 2) * (sizeof(char **)));
	if (!new_dict)
		return (NULL);
	i_entry = 0;
	while (dict[i_entry])
	{
		if (!ft_entry(i_entry, new_dict, dict))
			return (NULL);
		i_entry++;
	}
	new_dict[i_entry] = malloc(sizeof(char *) * 3);
	if (!new_dict[i_entry])
		return (NULL);
	new_dict[i_entry][0] = ft_strdup(key);
	new_dict[i_entry][1] = ft_strdup(value);
	new_dict[i_entry][2] = NULL;
	new_dict[i_entry + 1] = NULL;
	ft_delete_dict(dict);
	return (new_dict);
}

char	***ft_addto_dict(char *key, char *value, char ***dict)
{
	if (!key)
	{
		ft_printf("error: key missing\n");
		return (NULL);
	}
	if (dict)
		return (ft_existing_dict(key, value, dict));
	else
		return (ft_new_dict(key, value));
}
