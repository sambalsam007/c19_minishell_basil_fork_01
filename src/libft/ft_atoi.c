/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 16:28:02 by bclaeys           #+#    #+#             */
/*   Updated: 2024/04/30 15:44:14 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *str)
{
	int					i;
	int					posneg;
	unsigned long long	temp_value;

	i = 0;
	posneg = 1;
	temp_value = 0;
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\r' || str[i] == '\n'
		|| str[i] == '\v' || str[i] == '\f')
		i++;
	if (str[i] == '-')
		posneg *= -1;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (ft_isdigit(str[i]))
	{
		temp_value = temp_value * 10 + (str[i] - 48);
		if (temp_value > LONG_MAX && posneg == -1)
			return (0);
		if (temp_value > LONG_MAX && posneg == 1)
			return (-1);
		i++;
	}
	return ((int)(temp_value *= posneg));
}
