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
	split_PATH = ft_split(ft_get_value("PATH", var_data->envvar), ':');
	if (!split_PATH)
		return (ft_printf("Error: malloc failed\n"), NULL);
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
				return (ft_printf("Error: couldn't close dir\n"), NULL);
			break;
		}
		if (closedir(directory))
			return (ft_printf("Error: couldn't close dir\n"), NULL);
		i++;
	}
	ft_free_split(split_PATH);
	if (!file_found)
		return (ft_printf("Error: %s not a command\n", command), 
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
			return (ft_free_split(new_array), write(2, "Error: mall", 9), NULL);
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

int	check_if_binary(t_var_data *var_data, 
						t_ast_node *ast_node)
{
	pid_t	pid;
	char 	*path_bin;
	char 	**tmp_array;
	char 	**envvar_array;
	int		pipe_fd[2];

	pipe_fd[0] = 0;
	pipe_fd[1] = 1;
	if (var_data->first_node_ast->pipe && pipe(pipe_fd) == -1)
		return (ft_printf("Error: pipe failed\n"), 1);
	if (var_data->first_node_ast->pipe)
		var_data->tmp_pipe[1] = dup(STDOUT_FILENO);
	if (!ft_strchr("/~.", ast_node->command[0]))
	{
		path_bin = check_and_create_path(var_data, ast_node->command);
		if (!path_bin)
			return (1);
		if (!path_bin[0])
			return (free(path_bin), 0);
	}
	else 
		path_bin = ft_strdup(ast_node->command);
	if (path_bin && var_data->error_checks->executor_level_syntax_error == true)
		return (0);
	tmp_array = add_cmd_to_argarray(ast_node->arguments, ast_node->command);
	if (!tmp_array)
		return (free(path_bin), ft_printf("Error: malloc\n"), 1);
	if (!tmp_array[0])
		return (free(path_bin), 0);
	envvar_array = envvardict_to_envvararray(var_data->envvar);
	pid = fork();
	if (pid == -1)
		return (ft_free_split(envvar_array), ft_free_split(tmp_array),
				free(path_bin), ft_printf("Error: couldn't fork\n"), 1);
	if (pid == 0)
	{
		if (check_pipe(var_data, ast_node, pipe_fd)
					|| (sighandler(var_data, CHILD))
					|| (execve(path_bin, tmp_array, envvar_array) == -1))
				return (var_data->error_checks->executor_level_syntax_error = true,
						free(path_bin), ft_free_split(tmp_array), 
						ft_printf("Error: executor\n"), 1);
	}
	else
		return (free(path_bin), var_data->tmp_pipe[0] = dup(pipe_fd[0]), 
				close(pipe_fd[1]), ft_free_split(envvar_array), 
				ft_free_split(tmp_array), 0);
	return (1);
}
