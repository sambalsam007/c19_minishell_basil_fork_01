/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_line_inteface.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 12:23:04 by bclaeys           #+#    #+#             */
/*   Updated: 2024/12/19 13:07:03 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include <readline/history.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>

#define ERROR_CONTINUE -1
#define ERROR_STOP 1

int	ms_lex_and_parse(t_var_data *var_data,
						t_error_checks *error_checks,
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
	return (0);
}

/* void	test_print_parser(t_var_data *var_data) */
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

static int	execute_logic(t_var_data *var_data)
{
	t_ast_node *tmp_node;
	int			error_flow;
	int			status;

	tmp_node = var_data->first_node_ast;
	error_flow = 0;
	if (var_data->first_node_ast->pipe)
		if (pipe(var_data->tmp_pipe) == ERROR_CONTINUE)
			return (ft_putstr_fd("Error: pipe failed\n", STDERR_FILENO), ERROR_STOP);
	while (tmp_node)
	{
		if (!tmp_node->command)
			return (var_data->error_checks->executor_level_syntax_error = true, 
					ft_putstr_fd("Error: no command\n", STDERR_FILENO), 0);
		error_flow = check_if_redir(var_data, tmp_node->redirect);
		if (error_flow)
			return (ERROR_STOP);
		if (error_flow == ERROR_CONTINUE)
			 break;
		error_flow = check_if_builtin(var_data, tmp_node);
		if (error_flow == ERROR_STOP)
			return (ERROR_STOP);
		if (error_flow == ERROR_CONTINUE && check_if_binary(var_data, tmp_node))
			return (ERROR_STOP);
		tmp_node = tmp_node->pipe;		
	} 
	while (wait(&status) > 0)
		;
	if (WIFEXITED(var_data->wstatus))
		var_data->wstatus = status;
	return (0);
}

int	ms_execute(t_var_data *var_data)
{
	if (execute_logic(var_data))
		return (ERROR_STOP);
	if (restore_fds(var_data))
		return (ERROR_STOP);
	return (0);
}

static int	clean_prompt(char **prompt)
{
	int	i;
	int	j;
	char *new_prompt;

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
	char			*prompt;
	int				error_flow;

	prompt = NULL;
	while (true)
	{
		big_free(var_data, prompt);
		init_error_data(var_data->error_checks);
		prompt = readline("\033[33mbazzels_minishell> \033[0m");
		add_history(prompt);
		if (!prompt)
			return (ft_printf("exit\n"), 0);
		if (clean_prompt(&prompt))	
			return (ft_printf("Error: malloc\n"), 1);
		if (!ft_strncmp(prompt, "exit", 5))
			break ;
		error_flow = ms_lex_and_parse(var_data, var_data->error_checks, prompt);
		if (error_flow == ERROR_CONTINUE)
			continue ;
		else if (error_flow == ERROR_STOP)
			return (ERROR_STOP);
		if (ms_execute(var_data))
			return (ERROR_STOP);
	}
	return (free(prompt), 0);
}
