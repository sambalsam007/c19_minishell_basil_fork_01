/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 15:18:48 by bclaeys           #+#    #+#             */
/*   Updated: 2024/10/24 16:30:31 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char **init_envvar_list(char **envp)
{
	int	i;
	char **envvar;

	i = 0;
	envvar = malloc(sizeof(char **));
	if (!envvar)
		return (NULL);
	while (envp[i])
	{
		envvar[i] = ft_strdup(envp[i]);
		if (!envvar[i])
		{
			// big free function
			return (NULL);
		}
		i++;
	}
	envvar[i] = NULL;
	// toevoegen: lege envp 
	return (envvar);
}
/*
char **change_envvar_list()
{
	return(0);
}
*/
