/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_envvars.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 13:05:47 by bclaeys           #+#    #+#             */
/*   Updated: 2025/01/07 13:06:03 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	***init_envvar_noenvp(void)
{
	char	*temp[4];

	temp[0] = "PWD=/nfs/homes/bclaeys/common_core/minishell";
	temp[1] = "SHLVL=1";
	temp[2] = "_=/usr/bin/env";
	temp[3] = NULL;
	return (ft_create_dict(temp, '='));
}

char	***init_envvar_list(char **envp)
{
	if (!envp[0])
		return (init_envvar_noenvp());//initialize env var, if not yet initialized
	return (ft_create_dict(envp, '='));//xx
}
