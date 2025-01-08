/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftpr_puthex.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 12:40:23 by bclaeys           #+#    #+#             */
/*   Updated: 2024/05/28 14:49:45 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ftpr_puthex(size_t h, char c)
{
	int		count;
	char	*symbols;
	char	*symbols_up;

	count = 0;
	symbols = "0123456789abcdef";
	symbols_up = "0123456789ABCDEF";
	if (h >= 16)
	{
		count += ftpr_puthex(h / 16, c);
		h = h % 16;
	}
	if (c == 'x')
		count += ftpr_putchar(symbols[h]);
	if (c == 'X')
		count += ftpr_putchar(symbols_up[h]);
	return (count);
}

size_t	ftpr_puthex_fd(int fd, size_t h, char c)
{
	int		count;
	char	*symbols;
	char	*symbols_up;

	count = 0;
	symbols = "0123456789abcdef";
	symbols_up = "0123456789ABCDEF";
	if (h >= 16)
	{
		count += ftpr_puthex_fd(fd, h / 16, c);
		h = h % 16;
	}
	if (c == 'x')
		count += ftpr_putchar_fd(fd, symbols[h]);
	if (c == 'X')
		count += ftpr_putchar_fd(fd, symbols_up[h]);
	return (count);
}
