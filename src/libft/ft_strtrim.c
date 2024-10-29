/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 18:20:16 by bclaeys           #+#    #+#             */
/*   Updated: 2024/05/01 14:22:54 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	isset(char c, char const *set)
{
	int	i;

	i = 0;
	while (set[i])
		if (set[i++] == c)
			return (1);
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*ss;
	int		len;
	int		start;
	int		end;
	int		i;

	i = 0;
	start = 0;
	end = 0;
	if (!s1 && !set[i])
		return (ft_strdup(""));
	while (s1[i] && (isset(s1[i++], set)))
		start++;
	i = ft_strlen(s1) - 1;
	while (s1[i] && (isset(s1[i--], set)))
		end++;
	len = ft_strlen(s1) - start - end;
	ss = ft_substr(s1, start, len);
	return (ss);
}
