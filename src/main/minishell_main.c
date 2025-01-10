/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_main.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 11:48:11 by bclaeys           #+#    #+#             */
/*   Updated: 2025/01/07 14:00:47 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	main(int ac, char **av, char **envp)// envp is de ENV variables van de huidige bash shell
{
	
	t_var_data	*var_data;

	(void)av; 
			// niet nodig
	if (ac != 1)
		return (1); 
			// mag geen args meekrijgen
	var_data = init_var_data(envp); 
			// initialiseer env var
	if (!var_data)
		return (1);
			// initialiseren envvar mislukt? exit minishell
	sighandler(var_data, MAIN_PROCESS);
			// signaal ctrl-c ctrl-d kunnen ontvangen
	if (ms_command_line_inteface(var_data))
		return (free_var_data(var_data), 1);
			// de prompt waarin je kan inputten
			// loopt totdat je de loop exit
	restore_tty(var_data);
	free_var_data(var_data);
	return (0);
}
/* TESTEN */
/* sowieso alles in valgrind testen op uninit values enzovoort */
/* errors naar STDERRROR */
