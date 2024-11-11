/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 12:18:15 by bclaeys           #+#    #+#             */
/*   Updated: 2024/11/08 16:21:36 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include "src/libft/libft.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

# define ERROR_NULL NULL

typedef enum {
	EXEC = 1,
	ARGUMENT,
	FLAG,
	PIPE,
	REDIRECT,
} e_token_type_enum;

/* ************************************************************************** */
/*                                      structs                               */
/* ************************************************************************** */

typedef struct s_token_node 
{
	char 				*token;
	int 				type;
	struct s_token_node	*next;
	struct s_token_node	*prev;
} 						t_token_node;

typedef struct s_var_data
{
	char 			***envvar_dict;
	t_token_node	*first_node_lexer;
} 	t_var_data;

/* ************************************************************************** */
/*                                      main                                  */
/* ************************************************************************** */

char		***init_envvar_list(char **envp);
char 		**change_envvar_list(char **old_envvar, char *string_to_add);
t_var_data	*init_var_data(char **envp);
void		*free_var_data(t_var_data *var_data);
void		free_envvar(char **envvar);
char 		***init_envvar_noenvp();

/* ************************************************************************** */
/*                                      error_and_free                        */
/* ************************************************************************** */

int			prompt_error_checks(char *prompt);
int			ft_print_error(char *string);
void		*ft_print_error_null(char *string);
void		free_lexer(t_token_node *first_node_lexer);

/* ************************************************************************** */
/*                                      cli                                   */
/* ************************************************************************** */

int			ms_command_line_inteface(t_var_data *var_data);

/* ************************************************************************** */
/*                                      lexer                                 */
/* ************************************************************************** */

t_token_node	*tokenizer(char *prompt, char ***envvar);
char			*whitespace_exception(char *prompt, int *index, char ***envvar);
char			*double_quote_handler(char *prompt, int *index,  char ***envvar);
char			*single_quote_handler(char *prompt, int *index);
char			*redirect_handler(char *prompt, int *index);

/* ************************************************************************** */
/*                                      parser                                */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                      executor                              */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                      sighandler                            */
/* ************************************************************************** */

#endif
