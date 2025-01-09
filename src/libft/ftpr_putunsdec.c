/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftpr_putunsdec.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 11:37:25 by bclaeys           #+#    #+#             */
/*   Updated: 2024/08/05 14:43:00 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ftpr_putunsdec(unsigned int d)
{
	int		count;
	char	*symbols;

	count = 0;
	symbols = "0123456789";
	if (d >= 10)
	{
		count += ftpr_putunsdec(d / 10);
		d = d % 10;
	}
	count += ftpr_putchar(symbols[d]);
	return (count);
}

size_t	ftpr_putunsdec_fd(int fd, unsigned int d)
{
	int		count;
	char	*symbols;

	count = 0;
	symbols = "0123456789";
	if (d >= 10)
	{
		count += ftpr_putunsdec_fd(fd, d / 10);
		d = d % 10;
	}
	count += ftpr_putchar_fd(fd, symbols[d]);
	return (count);
}
