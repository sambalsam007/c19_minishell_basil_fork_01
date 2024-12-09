/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sighandler.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 10:40:45 by bclaeys           #+#    #+#             */
/*   Updated: 2024/12/05 13:06:46 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include <bits/types/siginfo_t.h>
#include <signal.h>
#include <unistd.h>

void	handle_signals(int sig, siginfo_t *info, void *context)
{
	if (sig == SIGINT)
		kill(getpid(), );
	if (sig == SIGQUIT)
	if (sig == EOF)
		return ;
}

int	sighandler(t_var_data *var_data)
{
	struct sigaction	signal_struct;

	signal_struct.sa_flags = SA_RESTART;
	signal_struct.sa_sigaction = &handle_signals;
	sigemptyset(&signal_struct.sa_mask);
	if (sigaction(SIGINT, &signal_struct, NULL))
		return (ft_printf("sigaction failed\n", 1));
	if (sigaction(SIGQUIT, &signal_struct, NULL))
		return (ft_printf("sigaction failed\n", 1));
	if (sigaction(EOF, &signal_struct, NULL))
		return (ft_printf("sigaction failed\n", 1));
	return (0);
}
