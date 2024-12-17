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
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <readline/readline.h>

int	homemade_getpid(void)
{
	int		fd;
	char 	*pid;
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

void	handle_signal_child(int sig, siginfo_t *info, void *context)
{
	if (info->si_code != SI_USER)
		return;
	if (sig == SIGINT)
	{
		exit(0);
		write(STDIN_FILENO, "\n", 1);	
		return;
	}
	if (sig == SIGQUIT)
		return;
	(void)context;
}

int	restore_tty(t_var_data *var_data)
{
	if (tcsetattr(STDIN_FILENO, TCSANOW, &var_data->original_termios) == -1)
		return (write(2, "Error: tcsetattr\n", 17), 1);
	return (0);
}

int	handle_signals_through_termios(t_var_data *var_data)
{
	struct termios termios_p;

	if ((var_data->termios_backup_check)) 
		return (0);
	if (tcgetattr(STDIN_FILENO, &var_data->original_termios) == -1)
		return (write(2, "Error: tcgetattr\n", 17), 1);
	termios_p = var_data->original_termios;
	termios_p.c_cc[VQUIT] = -1;
	termios_p.c_lflag &= ~ECHOCTL;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &termios_p) == -1)
		return (write(2, "Error: tcsetattr\n", 17), 1);
	return (0);
}

void	handle_signal_heredoc(int sig, siginfo_t *info, void *context)
{
	if (info->si_code != SI_USER)
		return;
	/* WATTTEFOOOOK */
	if (sig == SIGINT)
		return;
	if (sig == SIGQUIT)
		return;
	(void)context;
}

void	handle_signal_parent(int sig, siginfo_t *info, void *context)
{
	if (info->si_code != SI_USER)
		return;
	if (sig == SIGINT)
	{
		write(STDIN_FILENO, "\n", 1);	
		rl_replace_line("", 1);
		rl_on_new_line();
		rl_redisplay();
		return;
	}
	if (sig == SIGQUIT)
		return;
	(void)context;
}

int	sighandler(t_var_data *var_data, int mode)
{
	struct sigaction	signal_struct;

	(void)var_data;
	signal_struct.sa_flags = SA_RESTART;
	signal_struct.sa_flags = SA_SIGINFO;
	if (mode == CHILD)
		signal_struct.sa_sigaction = handle_signal_child;
	else if (mode == HERE_DOC)
		signal_struct.sa_sigaction = handle_signal_heredoc;
	else
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

