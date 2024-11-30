/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 15:26:09 by bclaeys           #+#    #+#             */
/*   Updated: 2024/11/30 15:14:59 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

static int	export_exceptions(t_var_data *var_data, 
								t_ast_node *ast_node,
								int	fd)
{
	int i;
	char **split;

	i = 0;
	if (!ast_node->arguments[0])
	{
		ft_print_dict(var_data->envvar, "=", fd);
		ft_print_dict(var_data->no_var_envvar, "=", fd);
		return (1);
	}
	while (ast_node->arguments[i])
	{
		split = ft_split(ast_node->arguments[i], '=');
		if (!split)
			return (ft_printf("Error: malloc failed"), 1);
		if (!split[1] && !ft_get_value(split[0], var_data->envvar) 
				&& !ft_get_value(split[0], var_data->no_var_envvar))
			var_data->no_var_envvar 
				= ft_addto_dict(split[0], " ", var_data->no_var_envvar);
		ft_free_split(split);
		i++;
	}
	return (0);
}

static int	ms_export_update_dicts(t_var_data *var_data, 
									char **arg, 
									int *j, 
									int *i)
{
	*j = 2;
	(*i)++;
	if ((ft_get_value(arg[0], var_data->envvar) && arg[1]) 
			|| (ft_get_value(arg[0], var_data->no_var_envvar) && arg[1]))
	{
		if (ft_get_value(arg[0], var_data->no_var_envvar))
			var_data->no_var_envvar 
				= ft_delete_from_dict(arg[0], var_data->no_var_envvar);
		else 
			var_data->envvar 
				= ft_delete_from_dict(arg[0], var_data->envvar);
		var_data->envvar = ft_addto_dict(arg[0], arg[1], var_data->envvar);
	}
	else 
		var_data->envvar = ft_addto_dict(arg[0], arg[1], var_data->envvar);
	if (!var_data->envvar)
		return (ft_printf("Error: malloc\n"), ft_free_split(arg), 1);
	ft_free_split(arg);
	return (0);
}

int	ms_export_multiple_separators(char **arg, int *j)
{
	char *tmp_arg;

	while (arg[*j])
	{
		tmp_arg = arg[1];
		arg[1] = ft_strjoin(arg[1], "=");
		if (!arg[1])
			return (0);
		free(tmp_arg);
		tmp_arg = arg[1];
		arg[1] = ft_strjoin(arg[1], arg[(*j)++]);
		if (!arg[1])
			return (1);
		free(tmp_arg);
	}
	return (0);
}

int	ms_export(t_var_data *var_data, 
				t_ast_node *ast_node, 
				int fd)
{
	char **arg;
	int	i;
	int	j;

	i = 0;
	j = 2;
	if (export_exceptions(var_data, ast_node, fd))
		return (1);
	while(ast_node->arguments[i])
	{
		arg = ft_split(ast_node->arguments[i], '=');
		if (!arg)
			return (ft_printf("Error: malloc\n"), 1);
		if (!arg[1])
		{
			i++;	
			ft_free_split(arg);
			continue;
		}
		if (arg[j])
			if (ms_export_multiple_separators(arg, &j)) 
				return (ft_printf("Error: malloc\n"), 1);
		if (ms_export_update_dicts(var_data, arg, &j, &i))
			return(1);
	}
	return (0);
}
