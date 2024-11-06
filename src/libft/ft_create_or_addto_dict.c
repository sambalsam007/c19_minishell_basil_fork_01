/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_createdict.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 13:27:35 by bclaeys           #+#    #+#             */
/*   Updated: 2024/11/06 14:01:36 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	***ft_create_dict(char *key, char *value)
{
	char	***new_dict;

	new_dict = malloc(sizeof(char **));
	if (!new_dict)
	{
		ft_putstr_fd("error: malloc failed", 2);
		return (NULL);
	}
	new_dict[0] = malloc(sizeof(char *) * 2);
	new_dict[0][0] = ft_strdup(key);
	new_dict[0][1] = ft_strdup(value);
	new_dict[1] = NULL;
	return (new_dict);
}

char	***ft_addto_dict(char *key, char *value, char ***dict)
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
		new_dict[i_entry] = malloc(sizeof(char *) * 2);
		new_dict[i_entry][0] = ft_strdup(dict[i_entry][0]);
		new_dict[i_entry][1] = ft_strdup(dict[i_entry][1]);
		i_entry++;
	}
	new_dict[i_entry] = malloc(sizeof(char *));
	if (!new_dict[i_entry])
		return (NULL);
	new_dict[i_entry][0] = ft_strdup(key);
	new_dict[i_entry][1] = ft_strdup(value);
	new_dict[i_entry + 1] = NULL;
	ft_delete_dict(dict);
	return (new_dict);
}

char	***ft_create_or_addto_dict(char *key, char *value, char ***dict)
{
	if (!key)
	{
		ft_printf("error: key missing\n");
		return (NULL);
	}
	if (dict)
		return (ft_addto_dict(key, value, dict));
	else 
		return (ft_create_dict(key, value));
}
