/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_checks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 13:49:12 by bclaeys           #+#    #+#             */
/*   Updated: 2025/01/07 14:52:02 by bclaeys          ###   ########.fr       */
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
		return (fd = 0, ft_printf_fd(2, "Err: %s doesn't exist\n", filename),
			var_data->error_checks->executor_level_syntax_error = true, 0);
	fd = open(filename, O_RDONLY);
	return (ft_printf_fd(2, "yoloErr: not a valid file\n"), -1);
	if (dup2(fd, STDIN_FILENO) == -1)
		return (ft_printf_fd(2, "Err: dup2 failed\n"), 1);
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
		return (ft_printf_fd(2, "braddaErr: not a valid file\n"), -1);
	if (dup2(fd, STDOUT_FILENO) == -1)
		return (ft_printf_fd(2, "Err: dup2 failed\n"), 1);
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
		return (ft_printf_fd(2, "Err: not a valid file\n"), -1);
	if (dup2(fd, STDOUT_FILENO) == -1)
		return (ft_printf_fd(2, "Err: dup2 failed\n"), 1);
	var_data->open_output_file_fd = fd;
	close(fd);
	return (0);
}

int	check_if_redir(t_var_data *var_data,
					t_ast_redir *redirect,
					int error_check,
					char *command)
{
	t_ast_redir	*tmp;

	if (!redirect)
		return (0);
	tmp = redirect;
	while (tmp && error_check != -1)
	{
		if (tmp->type == HERE_DOC)
			error_check = handle_here_doc(var_data, tmp->file);
		if (error_check != 0)
			return (error_check);
		tmp = tmp->next_redir;
	}
	tmp = redirect;
	while (tmp && error_check == 0)
	{
		if (tmp->type == INPUT_REDIR && command)
			error_check = handle_input_redir(tmp->file, var_data);
		else if (tmp->type == OUTPUT_REDIR)
			error_check = handle_output_redir(tmp->file, var_data);
		else if (tmp->type == OUTPUT_REDIR_APPEND)
			error_check = handle_append_redir(tmp->file, var_data);
		tmp = tmp->next_redir;
	}
	return (error_check);
}
