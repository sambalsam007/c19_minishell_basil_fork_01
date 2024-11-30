/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_and_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 11:58:16 by bclaeys           #+#    #+#             */
/*   Updated: 2024/11/30 13:12:00 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"
#include <unistd.h>

void	ms_pwd(t_var_data *var_data, int fd)
{
	ft_putstr_fd(ft_get_value("PWD", var_data->envvar), fd);
	write(1, "\n", fd);
}

/* getcwd(NULL, 0) will not work on other OSes probably  */
int		ms_cd(t_var_data *var_data, t_ast_node *ast_node)
{
	if (ast_node->arguments[1])
	{
		var_data->error_checks->parser_level_syntax_error = true;	
		ft_printf("Error: too many arguments\n", 2);
		return (1);
	}
	if (chdir(ast_node->arguments[0]) == -1)
		return (ft_printf("Error: chdir\n", 2), 1);
	ft_update_dict("PWD", getcwd(NULL, 0), var_data->envvar);
	return (0);	
}