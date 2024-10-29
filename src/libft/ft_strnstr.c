/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 15:47:08 by bclaeys           #+#    #+#             */
/*   Updated: 2024/04/30 15:54:36 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	check_substr(const char *b, const char *l, size_t i, size_t len)
{
	size_t	j;

	j = 0;
	while (b[i] == l[j] && i < len && b[i])
	{
		j++;
		i++;
	}
	if (!l[j])
		return (1);
	return (0);
}

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;

	i = 0;
	if (!little[i])
		return ((char *)(big));
	if (!len)
		return (0);
	while (big[i] != little[0] && big[i])
		i++;
	while (big[i])
	{
		if (check_substr(big, little, i, len))
			return ((char *)(&big[i]));
		else
			i++;
	}
	return (0);
}
