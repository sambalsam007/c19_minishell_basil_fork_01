/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sighandler.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 10:40:45 by bclaeys           #+#    #+#             */
/*   Updated: 2025/01/07 14:38:22 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include <bits/types/siginfo_t.h>
#include <fcntl.h>
#include <readline/readline.h>
#include <signal.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

int	homemade_getpid(void)
{
	int		fd;
	char	*pid;
	int		pid_int;

	pid = malloc(sizeof(char) * 8);
	fd = open("/proc/self/stat", O_RDONLY);
	read(fd, pid, 7);
	pid[7] = 0;
	pid_int = ft_atoi(pid);
	free(pid);
	close(fd);
	return (pid_int);
}

void	handle_signal_heredoc(int sig, siginfo_t *info, void *context)
{
	if (info->si_code != SI_USER)
		return ;
	if (sig == SIGINT)
	{
		write(STDIN_FILENO, "\n", 1);
		kill(homemade_getpid(), SIGTERM);
	}
	(void)context;
}

void	handle_signal_parent(int sig, siginfo_t *info, void *context)
{
	if (info->si_code != SI_USER)
		return ;
	if (sig == SIGINT)
	{
		write(STDIN_FILENO, "\n", 1);
		rl_replace_line("", 1);
		rl_on_new_line();
		rl_redisplay();
		return ;
	}
	(void)context;
}

void	handle_signal_child(int sig, siginfo_t *info, void *context)
{
	if (info->si_code != SI_USER)
		return ;
	if (sig == SIGINT)
	{
		exit(0);
		write(STDIN_FILENO, "\n", 1);
		return ;
	}
	(void)context;
}

int	sighandler(t_var_data *var_data, int mode)
{
	struct sigaction	signal_struct;

	signal_struct.sa_flags = SA_RESTART;
	signal_struct.sa_flags = SA_SIGINFO;
	if (mode == EXECUTOR)
		signal_struct.sa_sigaction = handle_signal_child;
	else if (mode == HERE_DOC)
		signal_struct.sa_sigaction = handle_signal_heredoc;
	else if (mode == MAIN_PROCESS)
	{
		if (handle_signals_through_termios(var_data) == 1)
			return (1);
		signal_struct.sa_sigaction = handle_signal_parent;
	}
	sigemptyset(&signal_struct.sa_mask);
	if (sigaction(SIGINT, &signal_struct, NULL))
		return (ft_printf("sigaction failed\n", 1));
	if (sigaction(SIGQUIT, &signal_struct, NULL))
		return (ft_printf("sigaction failed\n", 1));
	return (0);
}
