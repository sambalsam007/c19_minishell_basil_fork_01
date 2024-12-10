/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_line_inteface.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 15:47:45 by bclaeys           #+#    #+#             */
/*   Updated: 2024/12/05 10:47:28 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include <readline/history.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>

int	ms_lex_and_parse(t_var_data *var_data,
						t_error_checks *error_checks,
						char *prompt)
{
	int	i;

	i = 0;
	while (prompt[i] && ft_iswhitespace(prompt[i]))
		i++;
	if (!prompt[0] || !prompt[i])
		return (-1);
	var_data->first_node_lexer = tokenizer(prompt, var_data,
			var_data->first_node_lexer, 0);
	if (error_checks->lexer_level_syntax_error == true)
		return (-1);
	if (!var_data->first_node_lexer)
		return (free(prompt), 1);
	if (parser(&var_data->first_node_ast, var_data->first_node_lexer, var_data))
		return (1);
	if (error_checks->parser_level_syntax_error == true)
		return (-1);
	return (0);
}

void	test_print_parser(t_var_data *var_data)
{
	int				i;
	t_ast_node		*tmp;
	t_ast_redir		*tmp_redir;

	i = 0;
	tmp = var_data->first_node_ast;
	while (tmp)
	{
		ft_printf("command: %s\n", tmp->command);
		while (tmp->arguments && tmp->arguments[i])
		{
			ft_printf("arguments: %s\n", tmp->arguments[i]);
			i++;	
		}
		i = 0;
		tmp_redir = tmp->redirect;
		while (tmp_redir)
		{
			ft_printf("redirects: type = %d, file = %s\n", 
					tmp_redir->type, tmp_redir->file);
			tmp_redir = tmp_redir->next_redir;
		}
		tmp = tmp->pipe;
		ft_printf("+++++\n");
	}
}

/* redirects kapotgemaakt */
static int	execute_logic(t_var_data *var_data)
{
	t_ast_node *tmp_node;
	int			error_check;

	tmp_node = var_data->first_node_ast;
	error_check = 0;
	if (var_data->first_node_ast->pipe)
		if (pipe(var_data->tmp_pipe) == -1)
			return (ft_printf("Error: pipe failed\n"), 1);
	while (tmp_node)
	{
		if (!tmp_node->command)
			return (var_data->error_checks->executor_level_syntax_error = true, 
					ft_printf("Error: no command\n"), 1);
		/* if (check_pipe(var_data, tmp_node->pipe)) */
		/* 	return (1); */
		if (check_if_redir(var_data, tmp_node->redirect))
			return (1);
		error_check = check_if_builtin(var_data, tmp_node);
		if (error_check == 1)
			return (1);
		if (error_check == -1 && check_if_binary(var_data, tmp_node))
			return (1);
		tmp_node = tmp_node->pipe;		
	} 
	while (wait(NULL) > 0)
		write (var_data->std_output_fd_backup, "wait\n", 5);//TEST
	return (0);
}

int	ms_execute(t_var_data *var_data)
{
	var_data->std_input_fd_backup = dup(STDIN_FILENO);
	if (var_data->std_input_fd_backup == -1)
		return (ft_printf("Error: dup failed\n"), 1);
	var_data->std_output_fd_backup = dup(STDOUT_FILENO);
	if (var_data->std_output_fd_backup == -1)
		return (ft_printf("Error: dup failed\n"), 1);
	if (execute_logic(var_data))
		return (1);
	/* if (restore_fds(var_data)) */
	/* 	return (1); */
	return (0);
}

int	ms_command_line_inteface(t_var_data *var_data)
{
	/* nog testen: een command dat niet bestaat heeft uninit value */
	/* env\ als command lekt */
	char			*prompt;
	int				flow_check;

	prompt = NULL;
	while (true)
	{
		big_free(var_data, prompt);
		init_error_data(var_data->error_checks);
		/* sighandler */
		prompt = readline("\033[33mbazzels_minishell> \033[0m");
		add_history(prompt);
		if (!prompt)
			return (ft_printf("exit\n"), 0);
		if (!ft_strncmp(prompt, "exit", 5))
			break ;
		flow_check = ms_lex_and_parse(var_data, var_data->error_checks, prompt);
		if (flow_check == -1)
			continue ;
		else if (flow_check == 1)
			return (1);
		/* test_print_parser(var_data); //TEST */
		if (ms_execute(var_data))
			return (1);
	}
	return (free(prompt), 0);
}
