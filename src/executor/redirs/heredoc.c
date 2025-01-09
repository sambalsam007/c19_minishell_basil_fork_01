/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 13:19:39 by bclaeys           #+#    #+#             */
/*   Updated: 2025/01/07 14:53:52 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define FATAL_ERROR 'f'
#define NON_FATAL_ERROR 'n'

static int	prompt_loop(char *prompt, char *filename, int redir_pipe_fd[2])
{
	while (prompt && ft_strncmp(prompt, filename, ft_strlen(filename) + 1)
		&& prompt[0] && prompt[0] != EOF && prompt[0] != '\4')
	{
		write(redir_pipe_fd[1], prompt, ft_strlen(prompt));
		write(redir_pipe_fd[1], "\n", 1);
		prompt = readline("\033[33m> \033[0m");
		if (!prompt || (prompt && prompt[0] == '\0'))
			return (1);
	}
	return (0);
}

static int	parent_process_continues(int redir_pipe_fd[2],
								t_var_data *var_data,
								int childpid)
{
	int	return_status;

	return_status = 100;// dit mag niet op 0 geinitialiseerd worden, want dat zorgt voor  onverwacht gedrag
	waitpid(childpid, &return_status, 0);
	WIFEXITED(return_status);
	if (WTERMSIG(return_status))
	{
		close(redir_pipe_fd[0]);
		var_data->last_error_code = WTERMSIG(return_status);
		return (-1);
	}
	else
	{
		dup2(redir_pipe_fd[0], STDIN_FILENO);
		close(redir_pipe_fd[1]);
		close(redir_pipe_fd[0]);
		return (0);
	}
	return (0);
}

static void	exit_with_error(t_var_data *var_data, char error)
{
	if (error == FATAL_ERROR)
		var_data->error_checks->fatal_error = true;
	if (error == NON_FATAL_ERROR)
		var_data->error_checks->executor_level_syntax_error = true;
	exit(1);
}

static int	finish_process_normally(int redir_pipe_fd[2])
{
	close(redir_pipe_fd[0]);
	close(redir_pipe_fd[1]);
	exit(0);
	return (1);
}

int	handle_here_doc(t_var_data *var_data, char *filename)
{
	int		redir_pipe_fd[2];
	int		pid;
	char	*prompt;

	if (pipe(redir_pipe_fd) == -1)
		return (1);
	pid = fork();
	if (pid == -1)
		return (ft_printf_fd(2, "Err: fork\n"), 1);
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		sighandler(var_data, HERE_DOC);
		prompt = readline("\033[33m> \033[0m");
		if (!prompt)
			exit_with_error(var_data, FATAL_ERROR);
		if (prompt[0] == '\0' || prompt_loop(prompt, filename, redir_pipe_fd))
			exit_with_error(var_data, NON_FATAL_ERROR);
		if (redir_pipe_fd[0] == -1)
			exit_with_error(var_data, FATAL_ERROR);
		return (finish_process_normally(redir_pipe_fd));
	}
	else
		return (parent_process_continues(redir_pipe_fd, var_data, pid));
}
