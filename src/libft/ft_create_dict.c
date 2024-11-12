/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_create_dict.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 17:14:52 by bclaeys           #+#    #+#             */
/*   Updated: 2024/11/07 19:10:11 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	free_extra_values(char **dict_entry, char *new_value)
{
	int	i;

	i = 2;
	while (dict_entry[i])
		free(dict_entry[i++]);
	dict_entry[1] = new_value;
	dict_entry[2] = NULL;
}

static int	multiple_separators(char **dict_entry)
{
	int		i;
	int		total_strlen;
	char	*new_value;
	char	*temp_value;

	i = 1;
	total_strlen = 0;
	while (dict_entry[i])
		total_strlen += ft_strlen(dict_entry[i++]);
	new_value = malloc(sizeof(char) * total_strlen + 1);
	if (!new_value)
		return (1);
	new_value[0] = 0;
	ft_strlcat(new_value, dict_entry[1], ft_strlen(dict_entry[1]));
	i = 2;
	while (dict_entry[i])
	{
		ft_strlcat(new_value, dict_entry[i], ft_strlen(new_value)
				+ ft_strlen(dict_entry[i]));
		i++;
	}
	temp_value = dict_entry[1];
	free_extra_values(dict_entry, new_value);
	free(temp_value);
	return (0);
}

char	***ft_create_dict(char **list, char separator)
{
	char	***dict;
	int		i;

	i = 0;
	while (list[i])
		i++;
	dict = malloc(sizeof(char **) * (i + 1));
	if (!dict)
		return (NULL);
	i = 0;
	while (list[i])
	{
		dict[i] = ft_split(list[i], separator);
		if (dict[i][2])
			if (multiple_separators(dict[i]))
				return (NULL);
		i++;
	}
	dict[i] = NULL;
	return (dict);
}
