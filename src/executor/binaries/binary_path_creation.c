/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   binary_path_creation.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 15:33:12 by bclaeys           #+#    #+#             */
/*   Updated: 2025/01/04 15:34:51 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

char *create_path_or_envp(char *directory_path, 
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

char *check_and_create_path(t_var_data *var_data, 
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
