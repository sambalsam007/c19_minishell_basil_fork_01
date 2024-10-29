/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_main.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 11:48:11 by bclaeys           #+#    #+#             */
/*   Updated: 2024/10/24 16:30:30 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	main(int ac, char **av, char **envp)
{
	t_var_data	var_data;
	int	i;

	i = 0;
	if (ac != 1)
		return (1);
	var_data.envvar = init_envvar_list(envp);
	while (envp[i])
	{
			ft_printf("%s\n", envp[i]);
			i++;
	}
	ft_printf("%-------------------------------\n");
	i = 0;
	while (var_data.envvar[i])
	{
			ft_printf("%s\n", var_data.envvar[i]);
			i++;
	}
	(void)av;
}
