/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 15:02:06 by bclaeys           #+#    #+#             */
/*   Updated: 2024/05/01 14:04:39 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_bytes(int n)
{
	int	n_copy;
	int	bytes;

	bytes = 0;
	if (n < 0)
	{
		n *= -1;
		bytes++;
	}
	n_copy = n;
	while (n_copy >= 10)
	{
		n_copy /= 10;
		bytes++;
	}
	return (bytes);
}

char	*ft_itoa(int n)
{
	char	*dest;
	int		n_copy;
	int		bytes;

	bytes = 0;
	n_copy = n;
	if (n == 0)
		return (ft_strdup("0"));
	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	bytes = count_bytes(n);
	dest = ft_calloc(sizeof(char), bytes + 2);
	if (!dest)
		return (0);
	if (n < 0)
		n *= -1;
	while (n > 0)
	{
		dest[bytes] = (n % 10) + 48;
		n /= 10;
		bytes--;
	}
	if (n_copy < 0)
		dest[bytes] = '-';
	return (dest);
}
