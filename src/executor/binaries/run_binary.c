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
#include <stdlib.h>
#include <unistd.h>

static char *create_path(char *directory_path, 
					char *command) 
{
	char *binary_path;
	char *binary_path_tmp;

	binary_path = ft_strjoin(directory_path, "/");
	if (!binary_path)
		return (ft_printf("malloc fail"), NULL);
	binary_path_tmp = binary_path;
	binary_path = ft_strjoin(binary_path, command);
	free(binary_path_tmp);
	if (!binary_path)
		return (ft_printf("malloc fail"), NULL);
	return (binary_path);
}


static char *check_and_create_path(t_var_data *var_data, char *command) 
{
	int 			i;
	char 			**split_PATH;
	char 			*binary_path;
	DIR 			*directory;
	struct dirent	*file_found;
	
	i = 0;
	split_PATH = ft_split(ft_get_value("PATH", var_data->envvar), ':');
	binary_path = NULL;
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
			binary_path = create_path(split_PATH[i], command);
			if (closedir(directory))
				return (ft_printf("Error: couldn't close dir\n"), NULL);
			break;
		}
		if (closedir(directory))
			return (ft_printf("Error: couldn't close dir\n"), NULL);
		i++;
	}
	if (!file_found)
		return (ft_printf("Error: %s not a comand\n", command), NULL);
	ft_free_split(split_PATH);
	return (binary_path);
}


int	check_if_binary(t_var_data *var_data, 
						t_ast_node *ast_node, 
						int fd,
						char **envp)
{
	int		pid;
	char 	*path_binary;


	(void)fd; //Hoe dit te combineren met execve?
	path_binary = check_and_create_path(var_data, ast_node->command);
	if (!path_binary)
		return (1);
	pid = fork();
	if (pid == -1)
		return (free(path_binary), ft_printf("Error: couldn't fork\n"), 1);
	if (pid == 0)
	{
		execve(path_binary, ast_node->arguments, envp);
		exit(0);
	}
	else
		wait(0);	
	free(path_binary);
	return (0);
}
