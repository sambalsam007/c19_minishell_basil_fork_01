/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftpr_putptr.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 11:37:25 by bclaeys           #+#    #+#             */
/*   Updated: 2024/08/05 14:42:43 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ftpr_putptr(size_t p)
{
	size_t	memory_adress;

	memory_adress = p;
	if (p == 0)
		return (ftpr_putstr("(nil)"));
	ftpr_putstr("0x");
	return (ftpr_puthex(memory_adress, 'x') + 2);
}
