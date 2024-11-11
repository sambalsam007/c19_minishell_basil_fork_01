/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_delete_dict.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 14:23:46 by bclaeys           #+#    #+#             */
/*   Updated: 2024/11/06 14:26:20 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_delete_dict(char ***dict)
{
	int	i_entry;
	int	j;

	i_entry = 0;
	j = 0;
	if (!dict)
		return ;
	while (dict[i_entry])
	{
		while (dict[i_entry][j])
		{
			free(dict[i_entry][j]);
			j++;
		}
		j = 0;
		free(dict[i_entry]);
		i_entry++;
	}
	free(dict);
	return ;
}
