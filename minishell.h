/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 12:18:15 by bclaeys           #+#    #+#             */
/*   Updated: 2024/12/04 17:44:03 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include "src/libft/libft.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/limits.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <termios.h>
#include <fcntl.h>
#include <readline/readline.h>
#include <readline/history.h>

# define ERROR_NULL NULL
#define CHILD 1
#define PARENT 3

typedef enum {
	EXEC = 1,
	ARGUMENT,
	FLAG,
	PIPE,
	REDIRECT,
} e_token_type_enum;

typedef enum {
	OUTPUT_REDIR_APPEND = 1,
	HERE_DOC,
	OUTPUT_REDIR,
	INPUT_REDIR,
} e_redir_type;

/* ************************************************************************** */
/*                                      structs                               */
/* ************************************************************************** */

typedef struct s_token_node 
{
	char 				*token;
	int 				type;
	bool 				argument_check;
	struct s_token_node	*next;
	struct s_token_node	*prev;
} 						t_token_node;

typedef struct s_ast_redir 
{
	char 				*file;
	int					type;
	struct s_ast_redir	*next_redir;
} 						t_ast_redir;

typedef struct s_ast_node 
{
	char 				*command;
	char				*flag;
	struct s_ast_node	*pipe;
	t_ast_redir			*redirect;
	char				**arguments;
} 						t_ast_node;

typedef struct s_error_checks
{
	bool				lexer_level_syntax_error;
	bool				parser_level_syntax_error;
	bool				executor_level_syntax_error;
}	t_error_checks;

typedef struct s_var_data
{
	char 			***envvar;
	char 			***no_var_envvar;
	t_token_node	*first_node_lexer;
	t_ast_node		*first_node_ast;
	t_error_checks  *error_checks;
	int				open_output_file_fd;
	int				open_input_file_fd;
	int				amount_of_pipes;
	int				tmp_pipe[2];
	int				std_output_fd_backup;
	int				std_input_fd_backup;
	bool			pipe_check;
	struct termios	original_termios;
	int				wstatus;
} 	t_var_data;


/* ************************************************************************** */
/*                                      main                                  */
/* ************************************************************************** */

char			***init_envvar_list(char **envp);
char 			**change_envvar_list(char **old_envvar, char *string_to_add);
t_var_data		*init_var_data(char **envp);
void			*free_var_data(t_var_data *var_data);
void			free_envvar(char **envvar);
char 			***init_envvar_noenvp();
void			init_error_data(t_error_checks *error_checks);

/* ************************************************************************** */
/*                                      error_and_free                        */
/* ************************************************************************** */

int				prompt_error_checks(t_token_node *first_node, t_error_checks *error_checks);
int				ft_print_error(char *string);
void			*ft_print_error_null(char *string);
void			free_lexer(t_token_node *first_node_lexer);
void			big_free(t_var_data *var_data, char *prompt);

/* ************************************************************************** */
/*                                      cli                                   */
/* ************************************************************************** */

int				ms_command_line_inteface(t_var_data *var_data);

/* ************************************************************************** */
/*                                      lexer                                 */
/* ************************************************************************** */

t_token_node	*tokenizer(char *prompt, t_var_data *var_data, t_token_node *first_nd, int flow_check); 
char			*redirect_handler(char *prompt, size_t *index, t_var_data *var_data);
int				single_quotes(char *prompt, size_t *index, char **tokenized_string);
int				double_quotes(char *prompt, size_t *index, t_var_data *var_data, char **token_str);
char			*ft_get_key(char *prompt);
int				ft_strtok(char *prompt, t_var_data *var_data, char **token, size_t *i);
t_token_node	*create_node(char *tokenized_str, t_token_node *prev_node, t_token_node *current_node);
size_t			check_if_join_args(t_var_data *var_data, char *prompt, char *tmp_str, t_token_node *current_node);
int				whitespace_exception(char *prompt, size_t *index, t_var_data *var_data, char **token);
int				no_quotes_arg(char *prompt, size_t *index, char ***envvar, char **token);
int				fill_token_expd_vars(char *prompt, char *token_string, char *key, char ***envvar);
int				check_single_dollar(char *prompt, size_t *index, char **token);
int				count_total_strlen(char *prompt, t_var_data *var_data, char *key, int *len_expanded_var);
int				count_key(char **key, char *prompt, int *i, int *j);
/* int				count_key(char *key, char *prompt, int *i, int *j); */

/* ************************************************************************** */
/*                                      parser                                */
/* ************************************************************************** */

int				parser(t_ast_node **first_ast_node, t_token_node *first_token_node, t_var_data *var_data);
t_ast_redir 	*add_redirect(t_token_node **curr_token_node, t_ast_redir *curr_redir_node, t_ast_redir *first_redir_node, t_var_data *var_data);
t_ast_node 		*create_ast_node(t_ast_node *prev_ast_node, t_token_node **curr_token_node,	t_var_data *var_data);

/* ************************************************************************** */
/*                                      executor                              */
/* ************************************************************************** */

int				check_if_binary(t_var_data *var_data, t_ast_node *ast_node);
int				check_if_redir(t_var_data *var_data, t_ast_redir *redirect);
int				check_if_builtin(t_var_data *var_data, t_ast_node *ast_node);
int				ms_echo(t_var_data *var_data, t_ast_node *ast_node);
int				ms_env(t_var_data *var_data, t_ast_node *ast_node);
int				ms_export(t_var_data *var_data, t_ast_node *ast_node);
int				ms_unset(t_var_data *var_data, t_ast_node *ast_node);
void			ms_pwd(t_var_data *var_data);
int				ms_cd(t_var_data *var_data, t_ast_node *ast_node);
int				restore_fds(t_var_data *var_data);
int				check_pipe(t_var_data *var_data, t_ast_node *curr_node_pipe, int *pipe_fd);

/* ************************************************************************** */
/*                                      sighandler                           */
/* ************************************************************************** */

int				sighandler(t_var_data *var_data, int mode);
int				restore_tty(t_var_data *var_data);

#endif
