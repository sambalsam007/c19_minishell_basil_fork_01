/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   big_free.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 12:35:17 by bclaeys           #+#    #+#             */
/*   Updated: 2024/10/30 13:42:48 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_print_error(char *string)
{
	//change ft_printf to take a fd? 
	ft_printf(string);
	return (1);
}

void	free_envvar(char **envvar)
{
	int	i;

	i = 0;
	while (envvar[i])
	{
		i++;		
		free(envvar[i - 1]);
	}
	free(envvar);
}

void	*free_var_data(t_var_data *var_data)
{
	if (var_data->envvar)
		free_envvar(var_data->envvar);
	if (var_data)
		free(var_data);
	return (NULL);
}
