/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_split.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 15:13:07 by bclaeys           #+#    #+#             */
/*   Updated: 2024/11/30 15:14:34 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void ft_free_split(char **split_arg)
{
	int	i;

	i = 0;
	while (split_arg[i])	
	{
		free(split_arg[i]);
		i++;
	}
	free(split_arg);
}
