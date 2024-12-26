/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_binary.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 14:09:43 by bclaeys           #+#    #+#             */
/*   Updated: 2024/11/30 16:17:26 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"
#include <dirent.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

static char	*create_path_or_envp(char *directory_path,
					char *command,
					char *separator)
{
	char	*binary_path;
	char	*binary_path_tmp;

	binary_path = ft_strjoin(directory_path, separator);
	if (!binary_path)
		return (ft_printf("malloc fail"), NULL);
	binary_path_tmp = binary_path;
	binary_path = ft_strjoin(binary_path, command);
	free(binary_path_tmp);
	if (!binary_path)
		return (ft_printf("malloc fail"), NULL);
	return (binary_path);
}

static char	*check_and_create_path(t_var_data *var_data,
								char *command)
{
	int				i;
	char			**split_path;
	char			*binary_path;
	DIR				*directory;
	struct dirent	*file_found;

	(T_check_if_binary) ? ft_printf("\t\t\t--- check_and_create_path\n"):0;
	i = 0;
	if (!ft_get_value("PATH", var_data->envvar))
		return (ft_putstr_fd("Error: PATH not set\n", STDERR_FILENO),
			var_data->error_checks->executor_level_syntax_error = true,
			ft_strdup(""));
	split_path = ft_split(ft_get_value("PATH", var_data->envvar), ':');
	if (!split_path)
		return (ft_putstr_fd("Error: malloc failed\n", STDERR_FILENO), NULL);
	while (split_path[i])
	{
		(T_check_if_binary) ? ft_printf("\t\t\t\tsplit_path: %s\n", split_path[i]):0;
		directory = opendir(split_path[i]);
		if (!directory)
		{
			i++;
			continue ;
		}
		file_found = readdir(directory);
		while (file_found)
		{
			if (!ft_strncmp(file_found->d_name, command, \
						ft_strlen(command) + 1))
				break ;
			file_found = readdir(directory);
		}
		if (file_found)
		{
			binary_path = create_path_or_envp(split_path[i], command, "/");
			(T_check_if_binary) ? ft_printf("\t\t\t\tbinary_path: %s\n", binary_path):0;
			if (closedir(directory))
				return (ft_putstr_fd("Error: couldn't close dir\n",
						STDERR_FILENO), NULL);
			break ;
		}
		if (closedir(directory))
				return (ft_putstr_fd("Error: couldn't close dir\n",
						STDERR_FILENO), NULL);
		i++;
	}
	ft_free_split(split_path);
	if (!file_found)
		return (ft_putstr_fd("Error: not a valid command\n", STDERR_FILENO),
			var_data->error_checks->executor_level_syntax_error = true,
			ft_strdup(""));
	return (binary_path);
}

/* void	test_print_parser2(t_var_data *var_data) */
/* { */
/* 	int				i; */
/* 	t_ast_node		*tmp; */
/* 	t_ast_redir		*tmp_redir; */
/**/
/* 	i = 0; */
/* 	tmp = var_data->first_node_ast; */
/* 	while (tmp) */
/* 	{ */
/* 		ft_printf("command: %s\n", tmp->command); */
/* 		while (tmp->arguments && tmp->arguments[i]) */
/* 		{ */
/* 			ft_printf("arguments: %s\n", tmp->arguments[i]); */
/* 			i++;	 */
/* 		} */
/* 		i = 0; */
/* 		tmp_redir = tmp->redirect; */
/* 		while (tmp_redir) */
/* 		{ */
/* 			ft_printf("redirects: type = %d, file = %s\n",  */
/* 					tmp_redir->type, tmp_redir->file); */
/* 			tmp_redir = tmp_redir->next_redir; */
/* 		} */
/* 		tmp = tmp->pipe; */
/* 		ft_printf("+++++\n"); */
/* 	} */
/* } */

static char	**envvardict_to_envvararray(char ***envvar)
{
	int		i;
	char 	**new_array;

	i = 0;
	while (envvar[i]) 
		i++;
	new_array = malloc(sizeof(char *) * (i + 1));
	if (!new_array)
		return (NULL);
	i = 0;
	while (envvar[i]) 
	{
		new_array[i] = create_path_or_envp(envvar[i][0], envvar[i][1], "=");
		if (!new_array[i])
			return (ft_free_split(new_array), write(STDERR_FILENO, 
						"Error: malloc", 9), NULL);
		i++;
	}	
	new_array[i] = 0;
	return (new_array);
}

static char	**add_cmd_to_argarray(char **args, char *command)
{
	char 	**new_array;
	int 	i;
	
	i = 0;
	while (args[i])
			i++;
	new_array = malloc(sizeof(char *) * (i + 2));
	if (!new_array)
		return (NULL);
	i = 0;
	new_array[0] = ft_strdup(command);	
	while (args[i])
	{
		new_array[i + 1] = ft_strdup(args[i]);	
		i++;
	}
	new_array[i + 1] = NULL; 
	return (new_array);	
}

int	handle_empty_path_bin(char *path_bin) // xxx new function
{
	if (!path_bin)
		return (1);
	if (!path_bin[0])
	{
		free(path_bin);
		return (0);
	}
	return (0);
}

int	handle_fork_fail(char **envvar_array, char **tmp_array, char *path_bin) // xxx new function
{
	ft_putstr_fd("Error: couldn't fork\n", STDERR_FILENO);
	ft_free_split(envvar_array);
	ft_free_split(tmp_array);
	free(path_bin);
	return (1);
}

int	child_fail(t_var_data *var_data, char *path_bin, char **tmp_array, char **envvar_array) // xxx new function
{
	var_data->error_checks->executor_level_syntax_error = true;	
	free(path_bin);
	ft_free_split(tmp_array);
	ft_free_split(envvar_array);
	ft_putstr_fd("Execve error: check your command \n", STDERR_FILENO);
	return (1);
}

int	parent_free_and_continue(t_var_data *var_data, char **envvar_array, \
		char **tmp_array, int pipe_fd[]) // xxx new function
{
	var_data->tmp_pipe[0] = dup(pipe_fd[0]);
	close(pipe_fd[1]);
	ft_free_split(envvar_array);
	ft_free_split(tmp_array);
	return (0);
}

typedef struct s_fork_env
{
	char 	*path_bin;
	char 	**tmp_array;
	char 	**envvar_array;
	int		pipe_fd[2];
	pid_t	pid;
}	t_fork_env;

// int	create_fork()
// {
//
// 	int		pipe_fd[2];
// 	if (pid == -1)
// 		return (handle_fork_fail(envvar_array, tmp_array, path_bin));
// 	if (pid == 0)
// 	{
// 		if (check_pipe(var_data, ast_node, pipe_fd)
// 					|| (sighandler(var_data, EXECUTOR))
// 					|| (execve(path_bin, tmp_array, envvar_array) == -1))
// 			return (child_fail(var_data, path_bin, tmp_array, envvar_array));
// 	}
// 	else
// 	{
// 		free(path_bin);
// 		return (parent_free_and_continue(var_data, envvar_array, tmp_array, pipe_fd));
// 	}
// }

int	check_if_binary(t_var_data *var_data, 
						t_ast_node *ast_node)
{
	char 	*path_bin;
	char 	**tmp_array;
	char 	**envvar_array;
	int		pipe_fd[2];
	pid_t	pid;

	(T_check_if_binary) ? ft_printf("\t\t=== ms_CLI // ms_execute // check_if_binary\n"):0;
	pipe_fd[0] = 0;
	pipe_fd[1] = 1;
	if (var_data->first_node_ast->pipe && pipe(pipe_fd) == -1)
		return (ft_putstr_fd("Error: pipe failed\n", STDERR_FILENO), 1);
	if (var_data->first_node_ast->pipe)
		var_data->tmp_pipe[1] = dup(STDOUT_FILENO);
	if (!ft_strchr("/~.", ast_node->command[0]))
	{
		path_bin = check_and_create_path(var_data, ast_node->command);
		/*if (!(path_bin) || !(path_bin[0])) // xxx aanpassing*/
		/*	return (handle_empty_path_bin(path_bin));*/
		if (!path_bin)
		{
			(T_check_if_binary) ? ft_printf("\t\t\treturn 1\n"):0;
			return (1);
		}
		if (!path_bin[0])
		{
			(T_check_if_binary) ? ft_printf("\t\t\treturn 0\n"):0;
			return (free(path_bin), 0);
		}
		/* REMOVE ===========
		 * ================== */
	}
	else 
		path_bin = ft_strdup(ast_node->command);
	(T_check_if_binary) ? ft_printf("\t\t\tpath_bin: %s\n",path_bin):0;
	pipe_fd[0] = 0;
	if (path_bin && var_data->error_checks->executor_level_syntax_error == true)
		return (0);
	tmp_array = add_cmd_to_argarray(ast_node->arguments, ast_node->command);
	if (!(tmp_array))
		return (free(path_bin), ft_putstr_fd("Error\n", STDERR_FILENO), 1);
	if (!(tmp_array[0]))
		return (free(path_bin), 0);
	envvar_array = envvardict_to_envvararray(var_data->envvar);
	// create_fork();
	// hier aparte fct van maken
	/* REMOVE ===========
	pid = fork();
	if (pid == -1)
	{
		return (handle_fork_fail(envvar_array, tmp_array, path_bin));
	}
	if (pid == 0)
	{
		if (check_pipe(var_data, ast_node, pipe_fd)
					|| (sighandler(var_data, EXECUTOR))
					|| (execve(path_bin, tmp_array, envvar_array) == -1))
			return (child_fail(var_data, path_bin, tmp_array, envvar_array));
	}
	else
	{
		free(path_bin);
		return (parent_free_and_continue(var_data, envvar_array, tmp_array, pipe_fd));
	}
	////
	 * ================== */
	pid = fork();
	if (pid == -1)
		return (ft_putstr_fd("Error: couldn't fork\n", STDERR_FILENO),
				ft_free_split(envvar_array), ft_free_split(tmp_array),
				free(path_bin), 1);
	if (pid == 0)
	{
		if (check_pipe(var_data, ast_node, pipe_fd)
					|| (sighandler(var_data, EXECUTOR))
					|| (execve(path_bin, tmp_array, envvar_array) == -1))
		{
			var_data->error_checks->executor_level_syntax_error = true;	
			free(path_bin);
			ft_free_split(tmp_array);
			ft_free_split(envvar_array);
			ft_putstr_fd("Execve error: check your command \n", STDERR_FILENO);
			return (1);
		}
	}
	else
		return (free(path_bin), var_data->tmp_pipe[0] = dup(pipe_fd[0]), 
				close(pipe_fd[1]), ft_free_split(envvar_array), 
				ft_free_split(tmp_array), 0);
	// tot hier 
	return (1);
}
