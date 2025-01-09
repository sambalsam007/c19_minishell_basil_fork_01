/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftpr_putstr.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 11:37:25 by bclaeys           #+#    #+#             */
/*   Updated: 2024/08/05 14:42:59 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ftpr_putstr(char *s)
{
	int	i;

	i = 0;
	if (!s)
		return (ftpr_putstr("(null)"));
	while (s[i])
	{
		write(1, &s[i], 1);
		i++;
	}
	return (i);
}

size_t	ftpr_putstr_fd(int fd, char *s)
{
	int	i;

	i = 0;
	if (!s)
		return (ftpr_putstr("(null)"));
	while (s[i])
	{
		write(fd, &s[i], 1);
		i++;
	}
	return (i);
}
