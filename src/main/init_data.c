/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 15:18:48 by bclaeys           #+#    #+#             */
/*   Updated: 2024/10/30 13:42:47 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int init_envvar_noenvp(char **envvar)
{
		envvar[0] = ft_strdup
			("fPWD=/nfs/homes/bclaeys/common_core/gits/github/minishell");
		// mag ik bovenstaande hardcoden?
		envvar[1] = ft_strdup("SHLVL=1");
		envvar[2] = ft_strdup("_=/usr/bin/env");
		if (!envvar[0] || !envvar[1] || !envvar[2])
			return (ft_print_error("Error: malloc failed\n"));
		return (0);
}

char **init_envvar_list(char **envp)
{
	int	i;
	char **envvar;

	i = 0;
	while (envp[i])
		i++;
	if (!envp[0])
		i = 3;
	envvar = malloc(sizeof(char *) * (i + 1));
	if (!envvar)
		return (ERROR_NULL);
	i = 0;
	if (!envp[0])
		if (init_envvar_noenvp(envvar))
			return (ERROR_NULL);
	while (envp[i])
	{
		envvar[i] = ft_strdup(envp[i]);
		if (!envvar[i])
			return (ERROR_NULL);
		i++;
	}
	if (!envp[0])
		envvar[3] = NULL;
	else
		envvar[i] = NULL;
	return (envvar);
}

// deze fct is nog niet getest, samen testen met export
char **change_envvar_list(char **old_envvar, char *string_to_add)
{
	char 	**new_envvar;
	int	i;

	i = 0;
	while (old_envvar[i])
		i++;
	new_envvar = malloc(sizeof(char *) * (i + 2));
	if (!new_envvar)
	{
		ft_print_error("Error: malloc failed\n");
		return (ERROR_NULL);
	}
	i = 0;
	while (old_envvar[i])
	{
		new_envvar[i] = old_envvar[i];
		i++;
	}
	new_envvar[i] = string_to_add; 
	new_envvar[i + 1] = NULL;
	free_envvar(old_envvar);
	return (new_envvar);
}

t_var_data	*init_var_data(char **envp)
{
	t_var_data	*var_data;

	var_data = malloc(sizeof(t_var_data));
	if (!var_data)
		return (ERROR_NULL);
	var_data->envvar = init_envvar_list(envp);
	if (!var_data->envvar)
	{
		free_var_data(var_data);
		return (ERROR_NULL);
	}
	return (var_data);
}
