/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 17:51:23 by bclaeys           #+#    #+#             */
/*   Updated: 2024/04/30 14:21:25 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	int		i;
	int		j;
	char	*ss;

	i = 0;
	j = 0;
	ss = malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!ss)
		return (0);
	while (s1[i])
	{
		ss[j] = s1[i];
		i++;
		j++;
	}
	i = 0;
	while (s2[i])
	{
		ss[j] = s2[i];
		i++;
		j++;
	}
	ss[j] = 0;
	return (ss);
}
