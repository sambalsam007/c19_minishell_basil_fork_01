/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_create_dict.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 17:14:52 by bclaeys           #+#    #+#             */
/*   Updated: 2025/01/07 12:59:11 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

static void	free_extra_values(char **dict_entry, char *new_value)
{
	int	i;

	i = 2;
	while (dict_entry[i])
		free(dict_entry[i++]);
	dict_entry[1] = new_value;
	dict_entry[2] = NULL;
}

static void	mulsep_logic(char *new_value, char *separ, char **dict_entry, int i)
{
	while (dict_entry[i])
	{
		ft_strlcat(new_value, separ, ft_strlen(new_value) + 2);
		ft_strlcat(new_value, dict_entry[i], ft_strlen(new_value)
			+ ft_strlen(dict_entry[i]) + 1);
		i++;
	}
	free(separ);
}

static int	multiple_separators(char **dict_entry, char sep)
{
	int		i;
	int		total_strlen;
	char	*separ;
	char	*new_value;
	char	*temp_value;

	i = 1;
	total_strlen = 0;
	separ = malloc(sizeof(char) * 2);
	if (!separ)
		return (1);
	separ[0] = sep;
	separ[1] = 0;
	while (dict_entry[i])
		total_strlen += ft_strlen(dict_entry[i++]) + 1;
	new_value = malloc(sizeof(char) * total_strlen + 2);
	if (!new_value)
		return (free(separ), 1);
	new_value[0] = 0;
	ft_strlcat(new_value, dict_entry[1], ft_strlen(dict_entry[1]));
	mulsep_logic(new_value, separ, dict_entry, 2);
	temp_value = dict_entry[1];
	return (free_extra_values(dict_entry, new_value), free(temp_value), 0);
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
		if (!dict[i])
			return (ft_delete_dict(dict), NULL);
		if (dict[i][1] && dict[i][2])
			if (multiple_separators(dict[i], separator))
				return (NULL);
		i++;
	}
	dict[i] = NULL;
	return (dict);
}
