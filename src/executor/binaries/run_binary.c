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

static char *create_path_or_envp(char *directory_path, 
					char *command,
					char *separator) 
{
	char *binary_path;
	char *binary_path_tmp;

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

static char *check_and_create_path(t_var_data *var_data, 
								char *command) 
{
	int 			i;
	char 			**split_PATH;
	char 			*binary_path;
	DIR 			*directory;
	struct dirent	*file_found;
	
	i = 0;
	if (!ft_get_value("PATH", var_data->envvar))
		return (ft_putstr_fd("Error: PATH not set\n", STDERR_FILENO), 
				var_data->error_checks->executor_level_syntax_error = true, 
				ft_strdup(""));
	split_PATH = ft_split(ft_get_value("PATH", var_data->envvar), ':');
	if (!split_PATH)
		return (ft_putstr_fd("Error: malloc failed\n", STDERR_FILENO), NULL);
	while (split_PATH[i])
	{
		directory = opendir(split_PATH[i]);
		if (!directory)
		{
			i++;
			continue;
		}
		file_found = readdir(directory);
		while (file_found)
		{
			if (!ft_strncmp(file_found->d_name, command, ft_strlen(command) + 1))
					break;
			file_found = readdir(directory);
		}
		if (file_found)
		{
			binary_path = create_path_or_envp(split_PATH[i], command, "/");
			if (closedir(directory))
				return (ft_putstr_fd("Error: couldn't close dir\n", 
							STDERR_FILENO), NULL);
			break;
		}
		if (closedir(directory))
				return (ft_putstr_fd("Error: couldn't close dir\n", 
							STDERR_FILENO), NULL);
		i++;
	}
	ft_free_split(split_PATH);
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

typedef struct	s_binary_env
{
	pid_t	pid;
	char 	*path_bin;
	char 	**tmp_array;
	char 	**envvar_array;
	int		pipe_fd[2];

}	t_binary_env;

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

int	parent_free_and_continue(t_var_data *var_data, t_binary_env b) // xxx new function
{
	free(b.path_bin);
	var_data->tmp_pipe[0] = dup(b.pipe_fd[0]);
	close(b.pipe_fd[1]);
	ft_free_split(b.envvar_array);
	ft_free_split(b.tmp_array);
	return (0);
}

void	handle_pipes(t_var_data *var_data)
{
	if (var_data->first_node_ast->pipe)
		var_data->tmp_pipe[1] = dup(STDOUT_FILENO);
}


int	check_if_binary(t_var_data *var_data, 
						t_ast_node *ast_node)
{
	t_binary_env	b;

	b.pipe_fd[0] = 0;
	b.pipe_fd[1] = 1;
	if (var_data->first_node_ast->pipe && pipe(b.pipe_fd) == -1)
		return (ft_putstr_fd("Error: pipe failed\n", STDERR_FILENO), 1);
	handle_pipes(var_data); // xxx aanpassing
	/* REMOVE ========
	if (var_data->first_node_ast->pipe)
		var_data->tmp_pipe[1] = dup(STDOUT_FILENO);
	 * ==============*/
	if (!ft_strchr("/~.", ast_node->command[0]))
	{
		b.path_bin = check_and_create_path(var_data, ast_node->command);
		if (!(b.path_bin) || !(b.path_bin[0])) // xxx aanpassing
			return (handle_empty_path_bin(b.path_bin));
	}
	else 
		b.path_bin = ft_strdup(ast_node->command);
	if (b.path_bin && var_data->error_checks->executor_level_syntax_error == true)
		return (0);
	b.tmp_array = add_cmd_to_argarray(ast_node->arguments, ast_node->command);
	if (!b.tmp_array)
		return (free(b.path_bin), ft_putstr_fd("Error\n", STDERR_FILENO), 1);
	if (!b.tmp_array[0])
		return (free(b.path_bin), 0);
	b.envvar_array = envvardict_to_envvararray(var_data->envvar);
	b.pid = fork();
	if (b.pid == -1)
		return (handle_fork_fail(b.envvar_array, b.tmp_array, b.path_bin));
	if (b.pid == 0)
	{
		if (check_pipe(var_data, ast_node, b.pipe_fd)
					|| (sighandler(var_data, EXECUTOR))
					|| (execve(b.path_bin, b.tmp_array, b.envvar_array) == -1))
			return (child_fail(var_data, b.path_bin, b.tmp_array, b.envvar_array));
	}
	else
		return (parent_free_and_continue(var_data, b));
	return (1);
}
