/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   binary_path_creation.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 15:33:12 by bclaeys           #+#    #+#             */
/*   Updated: 2025/01/04 18:59:10 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"
#include <errno.h>

char	*create_path_or_envp(char *directory_path,
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

static int	path_error_checks(t_var_data *var_data,
								DIR *directory,
								struct dirent *file_found,
								char **split_PATH)
{
	ft_free_split(split_PATH);
	if (!directory && errno == ENOENT)
	{
		errno = 0;
		ft_putstr_fd("Error: no valid PATH set\n", STDERR_FILENO);
		var_data->error_checks->executor_level_syntax_error = true;
		return (1);
	}
	if (!file_found)
	{
		ft_putstr_fd("Error: not a valid command\n", STDERR_FILENO);
		var_data->error_checks->executor_level_syntax_error = true;
		return (1);
	}
	return (0);
}

static int	search_for_file(char *command,
							struct dirent **file_found,
							DIR *directory)
{
	if (errno != 0)
		return (ft_putstr_fd("Error: readdir\n",
				STDERR_FILENO), 1);
	while (*file_found)
	{
		if (!ft_strncmp((*file_found)->d_name, command, ft_strlen(command) + 1))
		{
			if (closedir(directory))
				return (ft_putstr_fd("Error: couldn't close dir\n",
						STDERR_FILENO), 1);
			return (-1);
		}
		*file_found = readdir(directory);
	}
	if (closedir(directory))
		return (ft_putstr_fd("Error: couldn't close dir\n",
				STDERR_FILENO), 1);
	return (0);
}

char	*path_creation_logic(char **split_PATH,
							DIR *directory,
							struct dirent **file_found,
							char *command)
{
	int	i;
	int	error_check;

	i = 0;
	while (split_PATH[i])
	{
		directory = opendir(split_PATH[i]);
		if (!directory)
		{
			i++;
			continue ;
		}
		errno = 0;
		*file_found = readdir(directory);
		error_check = search_for_file(command, file_found, directory);
		if (error_check == 1)
			return (NULL);
		if (error_check == -1)
			return (create_path_or_envp(split_PATH[i], command, "/"));
		i++;
	}
	return (NULL);
}

char	*check_and_create_path(t_var_data *var_data,
							char *command)
{
	char			**split_path;
	char			*binary_path;
	DIR				*directory;
	struct dirent	*file_found;

	file_found = NULL;
	directory = NULL;
	if (!ft_get_value("PATH", var_data->envvar))
		return (ft_putstr_fd("Error: PATH not set\n", STDERR_FILENO),
			var_data->error_checks->executor_level_syntax_error = true, NULL);
	split_path = ft_split(ft_get_value("PATH", var_data->envvar), ':');
	if (!split_path)
		return (ft_putstr_fd("Error: malloc failed\n", STDERR_FILENO), NULL);
	binary_path = path_creation_logic(split_path, directory, &file_found,
			command);
	if (path_error_checks(var_data, directory, file_found, split_path))
		return (NULL);
	return (binary_path);
}
