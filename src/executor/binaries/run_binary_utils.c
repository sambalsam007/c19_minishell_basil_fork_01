/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_binary_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 19:08:42 by bclaeys           #+#    #+#             */
/*   Updated: 2025/01/07 19:14:20 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"
#include <unistd.h>

char	**envvardict_to_envvararray(char ***envvar)
{
	int		i;
	char	**new_array;

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
			return (ft_free_split(new_array), \
					ft_printf_fd(2, "Err: malloc"), NULL);
		i++;
	}
	new_array[i] = 0;
	return (new_array);
}

char	**add_cmd_to_argarray(char **args, char *command)
{
	char	**new_array;
	int		i;

	i = 0;
	while (args[i])
		i++;
	new_array = malloc(sizeof(char *) * (i + 2));
	if (!new_array)
		return (NULL);
	i = 0;
	new_array[0] = ft_strdup(command);
	if (!new_array[0])
		return (free(new_array), NULL);
	while (args[i])
	{
		new_array[i + 1] = ft_strdup(args[i]);
		if (!new_array[i + 1])
		{
			i = 0;
			while (new_array[i])
				free(new_array[i++]);
		}
		i++;
	}
	return (new_array[i + 1] = NULL, new_array);
}

void	free_locals_executor(char *path_bin,
									char **envvar_array,
									char **tmp_array)
{
	free(path_bin);
	ft_free_split(tmp_array);
	ft_free_split(envvar_array);
}

int	set_fds_and_continue_parent(t_var_data *var_data,
									int pipe_fd[2])
{
	if (!isatty(var_data->tmp_pipe[0]))
		close(var_data->tmp_pipe[0]);
	var_data->tmp_pipe[0] = pipe_fd[0];
	if (!isatty(pipe_fd[1]))
		close(pipe_fd[1]);
	return (0);
}

int	tmp_argarray_error_checks(char **tmp_arg_array,
									char **envvar_array,
									char *path_bin)
{
	if (!tmp_arg_array)
	{
		free(path_bin);
		ft_free_split(envvar_array);
		return (ft_printf_fd(2, "Err\n"), 1);
	}
	if (!tmp_arg_array[0])
		return (free_locals_executor(path_bin, envvar_array, tmp_arg_array), 0);
	return (0);
}
