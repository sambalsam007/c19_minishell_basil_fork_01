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

size_t	check_if_join_args(char ***envvar, char *prompt, char *tmp_str,
		t_token_node *current_node)
{
	char	*tmp_str_extension;
	char	*tmp_str_ptr_storage;
	size_t	i;

	i = 0;
	while (!ft_iswhitespace(prompt[i]) && !ft_strchr("><", prompt[i]))
	{
		ft_strtok(&prompt[i], envvar, &tmp_str_extension, &i);
		tmp_str_ptr_storage = tmp_str;
		tmp_str = ft_strjoin(tmp_str, tmp_str_extension);
		if (!tmp_str)
			return (i);
		current_node = create_node(tmp_str, current_node, current_node);
		free(tmp_str_extension);
		free(tmp_str_ptr_storage);
	}
	return (i);
}
