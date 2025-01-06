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
	int	fd;

	if (access(filename, 0) == -1)
		return (fd = 0, ft_printf("Error: file %s doesn't exist", filename),
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
	int	fd;

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
	int	fd;

	fd = open(filename, O_RDWR | O_APPEND | O_CREAT, 0666);
	if (fd == -1)
		return (1);
	if (dup2(fd, STDOUT_FILENO) == -1)
		return (ft_printf("Error: dup2 failed\n"), 1);
	var_data->open_output_file_fd = fd;
	close(fd);
	return (0);
}

static int	handle_here_doc(t_var_data *var_data, char *filename)
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
			return (1);
		if (prompt[0] == '\0')
			return (-1);
		while (prompt && ft_strncmp(prompt, filename, ft_strlen(filename) + 1)
			&& prompt[0] && prompt[0] != EOF && prompt[0] != '\4')
		{
			write(redir_pipe_fd[1], prompt, ft_strlen(prompt));
			write(redir_pipe_fd[1], "\n", 1);
			prompt = readline("\033[33m> \033[0m");
			if (prompt && prompt[0] == '\0')
				return (-1);
		}
		sighandler(var_data, MAIN_PROCESS);
		redir_pipe_fd[0] = dup(STDIN_FILENO);
		if (redir_pipe_fd[0] == -1)
			return (ft_printf("Error: dup2 failed\n"), 1);
		close(redir_pipe_fd[0]);
		close(redir_pipe_fd[1]);
		exit(0);
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
