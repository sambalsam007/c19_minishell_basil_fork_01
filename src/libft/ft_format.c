/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_format.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 14:06:18 by bclaeys           #+#    #+#             */
/*   Updated: 2024/08/05 14:42:13 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_format_and_count(char s, va_list ap, size_t *count)
{
	if (s == 'c')
		*count += ftpr_putchar(va_arg(ap, int));
	else if (s == 's')
		*count += ftpr_putstr(va_arg(ap, char *));
	else if (s == 'i' || s == 'd')
		*count += ftpr_putint(va_arg(ap, int));
	else if (s == 'p')
		*count += ftpr_putptr(va_arg(ap, size_t));
	else if (s == 'u')
		*count += ftpr_putunsdec(va_arg(ap, unsigned int));
	else if (s == 'x' || s == 'X')
		*count += ftpr_puthex(va_arg(ap, unsigned int), s);
	else if (s == '%')
		*count += ftpr_putchar('%');
}
