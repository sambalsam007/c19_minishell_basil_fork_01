/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_checks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 13:49:12 by bclaeys           #+#    #+#             */
/*   Updated: 2024/12/04 17:37:57 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>

static int	handle_input_redir(char *filename, 
							t_var_data *var_data)
{
	int fd;

	if (access(filename, 0) == -1)
		return(fd = 0, ft_printf("Error: file %s doesn't exist", filename), 
				var_data->error_checks->executor_level_syntax_error = true, 0);
	fd = open(filename, O_RDONLY);
	if (fd == -1)
		return (1);
	if (dup2(fd, STDIN_FILENO) == -1)
		return (ft_printf("Error: dup2 failed\n"), 1);
	var_data->open_input_file_fd = fd;	
	close(fd);
	return (0);
}

static int	handle_output_redir(char *filename, 
							t_var_data *var_data)
{
	int fd;

	fd = open(filename, O_WRONLY | O_TRUNC | O_CREAT, 0666);
	if (fd == -1)
		return (1);
	if (dup2(fd, STDOUT_FILENO) == -1)
		return (ft_printf("Error: dup2 failed\n"), 1);
	var_data->open_output_file_fd = fd;
	close(fd);
	return (0);
}

static int	handle_append_redir(char *filename, 
							t_var_data *var_data)
{
	int fd;

	fd = open(filename, O_RDWR | O_APPEND | O_CREAT, 0666);
	if (fd == -1)
		return (1);
	if (dup2(fd, STDOUT_FILENO) == -1)
		return (ft_printf("Error: dup2 failed\n"), 1);
	var_data->open_output_file_fd = fd;
	close(fd);
	return (0);
}

typedef struct	s_heredoc_env
{
	int	redir_pipe_fd[2];
	int pid;
	char *prompt;
	int	empty_prompt_code;
}	t_heredoc_env;

// xxx new
int	first_readline(char **prompt)
{
	*prompt = readline("\033[33m> \033[0m");
	if (!*prompt)
		return (1);
	if (*prompt[0] == '\0')
		return (-1);
	return (0);
}

// xxx new
// i only tested basic pipe 
// dont know how to test pipe edge cases
int	reprompt(int redir_pipe_fd[], char **prompt)
{
	write(redir_pipe_fd[1], *prompt, ft_strlen(*prompt));
	write(redir_pipe_fd[1], "\n", 1);

	*prompt = readline("\033[33m> \033[0m");
	if (*prompt && *prompt[0] == '\0')
		return (-1);
	return (0);
}

// xxx new
int	redir_and_cleanup(int redir_pipe_fd[])
{
	redir_pipe_fd[0] = dup(STDIN_FILENO);
	if (redir_pipe_fd[0] == -1)
	{
		ft_printf("Error: dup2 failed\n");
		return (1);
	}
	close(redir_pipe_fd[0]);
	close(redir_pipe_fd[1]);
	exit (0);
}

/*
	memory leak
	with this test
		echo <<EOF
		111
		222
		EOF
	normally should be able to add a `333`
 */
static int handle_here_doc(t_var_data *var_data, char *filename)
{
	t_heredoc_env	h;
	
	if (pipe(h.redir_pipe_fd) == -1)
		return (1);
	h.pid = fork();
	if (h.pid == -1)
		return (ft_printf("Error: fork\n"), 1);
	if (h.pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		sighandler(var_data, HERE_DOC);
		if ((h.empty_prompt_code = first_readline(&h.prompt)) != 0)
			return (h.empty_prompt_code);
		while (h.prompt && ft_strncmp(h.prompt, filename, ft_strlen(filename) + 1)
				&& h.prompt[0] && h.prompt[0] != EOF && h.prompt[0] != '\4') 
		{
			if (reprompt(h.redir_pipe_fd, &h.prompt) == -1) // xxx
				return (-1);
		}
		sighandler(var_data, MAIN_PROCESS);
		if (redir_and_cleanup(h.redir_pipe_fd) == 1) // xxx
			return (1);
	}
	else
		wait(0);
	return (0);
}

int	check_if_redir(t_var_data *var_data, t_ast_redir *redirect)
{
	t_ast_redir	*tmp;
	int			error_check;

	error_check = 0;
	if (!redirect)
		return (0);
	tmp = redirect;
	while (tmp && error_check != 1)
	{
		if (tmp->type == HERE_DOC)
			error_check = handle_here_doc(var_data, tmp->file);
		tmp = tmp->next_redir;
	}
	tmp = redirect;
	while (tmp && error_check == 0)
	{
		if (tmp->type == INPUT_REDIR)
			error_check = handle_input_redir(tmp->file, var_data);
		else if (tmp->type == OUTPUT_REDIR)
			error_check = handle_output_redir(tmp->file, var_data);
		else if (tmp->type == OUTPUT_REDIR_APPEND)
			error_check = handle_append_redir(tmp->file, var_data);
		tmp = tmp->next_redir;
	}
	return (error_check);
}
