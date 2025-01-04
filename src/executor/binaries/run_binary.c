/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_binary.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 14:09:43 by bclaeys           #+#    #+#             */
/*   Updated: 2025/01/04 16:03:36 by bclaeys          ###   ########.fr       */
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

static void free_path_and_arrays(char *path_bin, 
								char **envvar_array, 
								char **tmp_array)
{
	free(path_bin);
	ft_free_split(tmp_array);
	ft_free_split(envvar_array);
}

static int fork_and_execute_child(t_var_data *var_data, 
						t_ast_node *ast_node,
						char **tmp_array,
						char *path_bin,
						int pipe_fd[2])
{
	pid_t	pid;
	char 	**envvar_array;

	envvar_array = envvardict_to_envvararray(var_data->envvar);
	pid = fork();
	if (pid == -1)
		return (ft_putstr_fd("Error: couldn't fork\n", STDERR_FILENO),
				free_path_and_arrays(path_bin, envvar_array, tmp_array), 1);
	if (pid == 0)
	{
		if (check_pipe(var_data, ast_node, pipe_fd)
				|| (sighandler(var_data, EXECUTOR)))
			exit(1);
		if (check_if_builtin(var_data, ast_node))
			exit(1);
		if (execve(path_bin, tmp_array, envvar_array) == -1)
		{
			var_data->error_checks->executor_level_syntax_error = true;	
			free_path_and_arrays(path_bin, envvar_array, tmp_array);
			ft_putstr_fd("Execve error: check your command \n", STDERR_FILENO);
			return (1);
		}
	}
	else
	{
		if (ast_node != var_data->first_node_ast)// new_basil
			close(var_data->tmp_pipe[0]);//new vbasil
		free_path_and_arrays(path_bin, envvar_array, tmp_array);
		return (var_data->tmp_pipe[0] = pipe_fd[0], close(pipe_fd[1]), 0);// can i call close the last node?
	}
	return (1);
}

int	execute_builtin_or_binary(t_var_data *var_data, 
						t_ast_node *ast_node)
{
	char 	*path_bin;
	char 	**tmp_array;
	int		pipe_fd[2];

	pipe_fd[0] = 0;
	pipe_fd[1] = 1;
	if (var_data->first_node_ast->pipe && ast_node->pipe && pipe(pipe_fd) == -1)//new
		return (ft_putstr_fd("Error: pipe failed\n", STDERR_FILENO), 1);
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
		return (free(path_bin), ft_putstr_fd("Error\n", STDERR_FILENO), 1);
	if (!tmp_array[0])
		return (free(path_bin), 0);
	return (fork_and_execute_child(var_data, ast_node, tmp_array, path_bin, pipe_fd));
}
