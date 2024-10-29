/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 12:06:42 by bclaeys           #+#    #+#             */
/*   Updated: 2024/04/25 17:29:15 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	int		i;
	char	*ptr;

	i = 0;
	ptr = NULL;
	c = (char)c;
	if (c == '\0')
	{
		i = ft_strlen(s);
		ptr = (char *)&s[i];
		return (ptr);
	}
	while (s[i] != c)
	{
		if (s[i] == 0)
			return (NULL);
		i++;
	}
	ptr = (char *)&s[i];
	return (ptr);
}
