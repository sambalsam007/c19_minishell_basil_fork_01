/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argument_concatenation.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 15:35:07 by bclaeys           #+#    #+#             */
/*   Updated: 2024/11/15 18:29:54 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int createnode_freetmps(char **tmp_str, 
		char *tmp_extension, 
		char *tmp_ptrstorage, 
		t_token_node *curr_node)
{
	*tmp_str = ft_strjoin(*tmp_str, tmp_extension);
	curr_node = create_node(*tmp_str, curr_node, curr_node);
	if (!curr_node)
		return (1);
	free(tmp_extension);
	free(tmp_ptrstorage);
	return (0);
}

size_t	check_if_join_args(t_var_data *var_data, char *prompt, char *tmp_str,
		t_token_node *curr_node)
{
	char	*tmp_extension;
	char	*tmp_ptrstorage;
	size_t	i;
	int		check;

	i = 0;
	if (!tmp_str)
		return (1);
	while (!ft_iswhitespace(prompt[i]) && !ft_strchr("|><", prompt[i]))
	{
		check = ft_strtok(&prompt[i], var_data, &tmp_extension, &i);
		if (check == 1)
			return (check);
		if (check == -1)
			return (var_data->error_checks->lexer_level_syntax_error = true, 1);
		if (!tmp_extension)
			continue;
		tmp_ptrstorage = tmp_str;
		if (createnode_freetmps(&tmp_str, tmp_extension, 
					tmp_ptrstorage, curr_node) == -1)
			return (-1);
		if (!*tmp_str)
			return (i);
	}
	return (i);
}
