/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_line_inteface.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 12:23:04 by bclaeys           #+#    #+#             */
/*   Updated: 2025/01/07 13:13:48 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include <readline/history.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define ERROR_CONTINUE -1
#define ERROR_STOP 1

/* void	test_print_parser(t_var_data *var_data) */
/* 	{ */
/* 		int				i; */
/* 		t_ast_node		*tmp; */
/* 		t_ast_redir		*tmp_redir; */
/**/
/* 		i = 0; */
/* 		tmp = var_data->first_node_ast; */
/* 		while (tmp) */
/* 		{ */
/* 			ft_printf("command: %s\n", tmp->command); */
/* 			while (tmp->arguments && tmp->arguments[i]) */
/* 			{ */
/* 				ft_printf("arguments: %s\n", tmp->arguments[i]); */
/* 				i++;		*/
/* 			} */
/* 			i = 0; */
/* 			tmp_redir = tmp->redirect; */
/* 			while (tmp_redir) */
/* 			{ */
/* 				ft_printf("redirects: type = %d, file = %s\n",  */
/* 						tmp_redir->type, tmp_redir->file); */
/* 				tmp_redir = tmp_redir->next_redir; */
/* 			} */
/* 			tmp = tmp->pipe; */
/* 			ft_printf("+++++\n"); */
/* 		} */
/* } */

int	ms_lex_and_parse(t_var_data *var_data, t_error_checks *error_checks,
		char *prompt)
{
	int	i;

	i = 0;
	while (prompt[i] && ft_iswhitespace(prompt[i]))
		i++;
	if (!prompt[0] || !prompt[i])
		return (ERROR_CONTINUE);
	var_data->first_node_lexer = tokenizer(prompt, var_data,
			var_data->first_node_lexer, 0);
	if (error_checks->lexer_level_syntax_error == true)
		return (ERROR_CONTINUE);
	if (!var_data->first_node_lexer)
		return (free(prompt), ERROR_STOP);
	if (parser(&var_data->first_node_ast, var_data->first_node_lexer, var_data))
		return (ERROR_STOP);
	if (error_checks->parser_level_syntax_error == true)
		return (ERROR_CONTINUE);
	if (expand_error_codes(var_data))
		return (ERROR_STOP);
	return (0);
}

int	ms_execute(t_var_data *var_data)
{
	int	error_flow;

	error_flow = execute_logic(var_data);
	if (error_flow != 0)
		return (error_flow);
	if (restore_fds(var_data))
		return (ERROR_STOP);
	return (0);
}

static int	clean_prompt(char **prompt)
{
	int		i;
	int		j;
	char	*new_prompt;

	i = 0;
	j = 0;
	while ((*prompt)[i])
	{
		if ((*prompt)[i] == '\\' || (*prompt)[i] == ';')
			j++;
		i++;
	}
	new_prompt = malloc(sizeof(char) * (i - j + 1));
	if (!new_prompt)
		return (1);
	i = 0;
	j = 0;
	while ((*prompt)[i])
	{
		if ((*prompt)[i] == '\\' || (*prompt)[i] == ';')
			i++;
		else
			new_prompt[j++] = (*prompt)[i++];
	}
	return (new_prompt[j] = 0, free(*prompt), *prompt = new_prompt, 0);
}

int	ms_command_line_inteface(t_var_data *var_data)
{
	char	*prompt;
	int		error_flow;

	prompt = NULL;
	while (true)
	{
		big_free(var_data, prompt);
		init_error_data(var_data, var_data->error_checks);
		prompt = readline("\033[33mbazzels_minishell> \033[0m");
		add_history(prompt);
		if (!prompt)
			return (ft_printf("exit\n"), 0);
		if (clean_prompt(&prompt))
			return (ft_printf_fd(2, "Err: malloc\n"), 1);
		if (!ft_strncmp(prompt, "exit", 5))
			break ;
		error_flow = ms_lex_and_parse(var_data, var_data->error_checks, prompt);
		if (error_flow == ERROR_CONTINUE)
			continue ;
		else if (error_flow == ERROR_STOP)
			return (big_free(var_data, prompt), ERROR_STOP);
		if (ms_execute(var_data) == ERROR_STOP)
			return (big_free(var_data, prompt), ERROR_STOP);
	}
	return (free(prompt), 0);
}
