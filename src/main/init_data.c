/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 15:18:48 by bclaeys           #+#    #+#             */
/*   Updated: 2024/11/07 19:07:15 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	***init_envvar_noenvp(void)
{
	char	*temp[4];

	// temp[0] aanpassen met getcwd()
	temp[0] = "PWD=/nfs/homes/bclaeys/common_core/minishell";
	temp[1] = "SHLVL=1";
	temp[2] = "_=/usr/bin/env";
	temp[3] = NULL;
	return (ft_create_dict(temp, '='));
}

char	***init_envvar_list(char **envp)
{
	if (!envp[0])
		return (init_envvar_noenvp());
	return (ft_create_dict(envp, '='));
}

t_var_data	*init_var_data(char **envp)
{
	t_var_data	*var_data;

	var_data = malloc(sizeof(t_var_data));
	if (!var_data)
		return (ft_print_error_null("Error: malloc failed\n"));
	if (!(var_data->envvar_dict = init_envvar_list(envp)))
	{
		free_var_data(var_data);
		return (ft_print_error_null("Error: malloc failedbla\n"));
	}
	var_data->first_node_lexer = NULL;
	return (var_data);
}
/* char *search_envvar_value(char **envvar, char *variable) */
/* { */
/* 	int		i; */
/* 	int		j; */
/**/
/* 	i = 0; */
/* 	j = 0; */
/* 	while (envvar[i]) */
/* 	{ */
/* 		while(envvar[i][j]) */
/* 		{ */
/* 			if (envvar[i][j] != variable[j]) */
/* 			{ */
/* 				j = 0; */
/* 				break ; */
/* 			} */
/* 			if (envvar[i][j] == '=') */
/* 				return (&envvar[i][j + 1]); */
/* 			j++; */
/* 		} */
/* 		i++; */
/* 	} */
/* 	return (ERROR_NULL); */
/* } */
/**/
/**/
/* char **init_envvar_list(char **envp) */
/* { */
/* 	int	i; */
/* 	char **envvar; */
/**/
/* 	i = 0; */
/* 	if (!envp[0]) */
/* 		i = 3; */
/* 	while (envp[i]) */
/* 		i++; */
/* 	envvar = malloc(sizeof(char *) * (i + 1)); */
/* 	if (!envvar) */
/* 		return (ft_print_error_null("Error: malloc failed\n"));		*/
/* 	i = 0; */
/* 	if (!envp[0]) */
/* 		if (init_envvar_noenvp(envvar)) */
/* 			return (ERROR_NULL); */
/* 	while (envp[i]) */
/* 	{ */
/* 		envvar[i] = ft_strdup(envp[i]); */
/* 		if (!envvar[i]) */
/* 			return (ERROR_NULL); */
/* 		i++; */
/* 	} */
/* 	if (!envp[0]) */
/* 		envvar[3] = NULL; */
/* 	else */
/* 		envvar[i] = NULL; */
/* 	return (envvar); */
/* } */
/**/
/* // deze fct is nog niet getest, samen testen met export */
/* char **change_envvar_list(char **old_envvar, char *string_to_add) */
/* { */
/* 	char 	**new_envvar; */
/* 	int	i; */
/**/
/* 	i = 0; */
/* 	while (old_envvar[i]) */
/* 		i++; */
/* 	new_envvar = malloc(sizeof(char *) * (i + 2)); */
/* 	if (!new_envvar) */
/* 	{ */
/* 		ft_print_error("Error: malloc failed\n"); */
/* 		return (ERROR_NULL); */
/* 	} */
/* 	i = 0; */
/* 	while (old_envvar[i]) */
/* 	{ */
/* 		new_envvar[i] = old_envvar[i]; */
/* 		i++; */
/* 	} */
/* 	new_envvar[i] = string_to_add;  */
/* 	new_envvar[i + 1] = NULL; */
/* 	free_envvar(old_envvar); */
/* 	return (new_envvar); */
/* } */
/**/
