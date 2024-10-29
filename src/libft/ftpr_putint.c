/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftpr_putint.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 11:37:25 by bclaeys           #+#    #+#             */
/*   Updated: 2024/08/05 14:42:36 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ftpr_putint(int n)
{
	int	neg_pos;
	int	count;

	neg_pos = 0;
	count = 0;
	if (n == -2147483648)
	{
		ftpr_putstr("-2147483648");
		return (11);
	}
	if (n < 0)
	{
		n *= -1;
		neg_pos = -1;
	}
	if (neg_pos == -1)
		count += ftpr_putchar('-');
	if (n > 9)
		count += ftpr_putint(n / 10);
	ftpr_putchar(n % 10 + 48);
	count++;
	return (count);
}
