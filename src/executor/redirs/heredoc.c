/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 13:19:39 by bclaeys           #+#    #+#             */
/*   Updated: 2025/01/07 14:00:49 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"
#include <stdio.h>

static int	prompt_loop(char *prompt, char *filename, int redir_pipe_fd[2])
{
	while (prompt && ft_strncmp(prompt, filename, ft_strlen(filename) + 1)
		&& prompt[0] && prompt[0] != EOF && prompt[0] != '\4')
	{
		write(redir_pipe_fd[1], prompt, ft_strlen(prompt));
		write(redir_pipe_fd[1], "\n", 1);
		prompt = readline("\033[33m> \033[0m");
		/* if (prompt && prompt[0] == '\0') */
		if (!prompt)
			return (1);
	}
	return (0);
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
		return (ft_printf("Error: fork\n"), 1);
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		sighandler(var_data, HERE_DOC);
		prompt = readline("\033[33m> \033[0m");
		if (!prompt)
		{
			var_data->error_checks->fatal_error = true;
			exit(1);
		}
		if (prompt[0] == '\0')
			return (-1);
		if (prompt_loop(prompt, filename, redir_pipe_fd))
			return (-1);
		sighandler(var_data, MAIN_PROCESS);
		redir_pipe_fd[0] = dup(STDIN_FILENO);
		if (redir_pipe_fd[0] == -1)
			return (ft_printf("Error: dup2 failed\n"), 1);
		/* close(redir_pipe_fd[0]); */
		close(redir_pipe_fd[1]);
		exit(0);
	}
	else
		wait(0);
	return (0);
}
