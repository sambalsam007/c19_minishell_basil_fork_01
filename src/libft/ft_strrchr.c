/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 19:06:22 by bclaeys           #+#    #+#             */
/*   Updated: 2024/04/25 17:37:30 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int		i;
	char	*ptr;

	i = ft_strlen(s);
	ptr = NULL;
	c = (char)c;
	if (c == '\0')
	{
		ptr = (char *)&s[ft_strlen(s)];
		return (ptr);
	}
	while (i != 0)
	{
		i--;
		if (s[i] == c)
		{
			ptr = (char *)&s[i];
			return (ptr);
		}
	}
	return (NULL);
}
