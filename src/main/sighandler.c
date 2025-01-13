/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sighandler.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 10:40:45 by bclaeys           #+#    #+#             */
/*   Updated: 2025/01/10 17:18:09 by bclaeys          ###   ########.fr       */
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
		ft_printf("test\n");
		kill(homemade_getpid(), SIGTERM);
	}
	(void)context;
}

void	handle_signal_parent(int sig, siginfo_t *info, void *context)
{
	if (info->si_code != SI_USER)
		return ;
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
		write(STDIN_FILENO, "\n", 1);
		exit(0);
	}
	(void)context;
}

int	sighandler(t_var_data *var_data, int mode)
{
	struct sigaction	signal_struct_sigint;
	struct sigaction	signal_struct_sigquit;

	signal_struct_sigint.sa_flags = SA_RESTART | SA_SIGINFO;
	signal_struct_sigquit.sa_flags = SA_RESTART | SA_SIGINFO;
	sigemptyset(&signal_struct_sigint.sa_mask);
	sigemptyset(&signal_struct_sigquit.sa_mask);
	if (mode == HERE_DOC)
		set_sigaction_for_heredoc(&signal_struct_sigint, &signal_struct_sigquit,
			&handle_signal_heredoc);
	if (mode == EXECUTOR)
		set_sigaction_for_exec(&signal_struct_sigint, &signal_struct_sigquit,
			&handle_signal_child);
	if (mode == MAIN_PROCESS)
	{
		if (fck_around_with_termios(var_data) == 1)
			return (1);
		set_sigaction_for_main(&signal_struct_sigint, &signal_struct_sigquit,
			&handle_signal_parent);
	}
	if (sigaction(SIGQUIT, &signal_struct_sigquit, NULL)
		|| sigaction(SIGINT, &signal_struct_sigint, NULL))
		return (ft_printf("sigaction failed\n", 1));
	return (0);
}
