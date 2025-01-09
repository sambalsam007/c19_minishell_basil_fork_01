/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 11:35:45 by bclaeys           #+#    #+#             */
/*   Updated: 2024/08/05 14:42:17 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_printf(const char *s, ...)
{
	va_list	ap;
	size_t	i;
	size_t	count;

	i = 0;
	count = 0;
	va_start(ap, s);
	if (!s)
		return (-1);
	while (s[i])
	{
		if (s[i] == '%')
		{
			i++;
			ft_format_and_count(s[i], ap, &count);
		}
		else
		{
			write(1, &s[i], 1);
			count++;
		}
		i++;
	}
	va_end(ap);
	return (count);
}

int	ft_printf_fd(int fd, const char *s, ...)
{
	va_list	ap;
	size_t	i;
	size_t	count;

	i = 0;
	count = 0;
	va_start(ap, s);
	if (!s)
		return (-1);
	while (s[i])
	{
		if (s[i] == '%')
		{
			i++;
			ft_format_and_count_fd(fd, s[i], ap, &count);
		}
		else
		{
			write(fd, &s[i], 1);
			count++;
		}
		i++;
	}
	va_end(ap);
	return (count);
}
