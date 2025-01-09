/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 12:18:15 by bclaeys           #+#    #+#             */
/*   Updated: 2025/01/07 19:13:52 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "src/libft/libft.h"
# include <stdbool.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <linux/limits.h>
# include <dirent.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <termios.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>

<<<<<<< HEAD
# define ERROR_NULL NULL
=======
>>>>>>> upstream/master
# define EXECUTOR 1
# define MAIN_PROCESS 3
# define LEXER_ERROR 999
# define PARSER_ERROR 998
# define EXECUTOR_ERROR 997
<<<<<<< HEAD

typedef enum
=======
# define ENV_ERROR 996

typedef enum s_token_type_enum
>>>>>>> upstream/master
{
	EXEC = 1,
	ARGUMENT,
	FLAG,
	PIPE,
	REDIRECT,
}	t_token_type_enum;

<<<<<<< HEAD
typedef enum
=======
typedef enum s_redir_type
>>>>>>> upstream/master
{
	OUTPUT_REDIR_APPEND = 1,
	HERE_DOC,
	OUTPUT_REDIR,
	INPUT_REDIR,
}	t_redir_type;

/* ************************************************************************** */
/*                                      structs                               */
/* ************************************************************************** */

typedef struct s_token_node
{
	char				*token;
	int					type;
	bool				argument_check;
	struct s_token_node	*next;
	struct s_token_node	*prev;
}	t_token_node;

typedef struct s_ast_redir
{
	char				*file;
	int					type;
	struct s_ast_redir	*next_redir;
}	t_ast_redir;

typedef struct s_ast_node
{
	char				*command;
	char				*flag;
	struct s_ast_node	*pipe;
	t_ast_redir			*redirect;
	char				**arguments;
}	t_ast_node;

typedef struct s_error_checks
{
	bool				lexer_level_syntax_error;
	bool				parser_level_syntax_error;
	bool				executor_level_syntax_error;
	bool				environment_error;
	bool				fatal_error;
}	t_error_checks;

typedef struct s_var_data
{
	bool			pipe_check;
	bool			is_redirect;
	bool			termios_backup_check;
	char			***envvar;
	char			***no_var_envvar;
	t_token_node	*first_node_lexer;
	t_ast_node		*first_node_ast;
	t_error_checks	*error_checks;
	int				open_output_file_fd;
	int				open_input_file_fd;
	int				amount_of_pipes;
	int				tmp_pipe[2];
	int				std_output_fd_backup;
	int				std_input_fd_backup;
	int				last_error_code;
	struct termios	original_termios;
}	t_var_data;

/* ************************************************************************** */
/*                                      main                                  */
/* ************************************************************************** */

char			***init_envvar_list(char **envp);
char			**change_envvar_list(char **old_envvar, char *string_to_add);
t_var_data		*init_var_data(char **envp);
void			*free_var_data(t_var_data *var_data);
void			free_envvar(char **envvar);
char			***init_envvar_noenvp(void);
void			init_error_data(t_var_data *var_data, \
<<<<<<< HEAD
								t_error_checks *error_checks);
=======
					t_error_checks *error_checks);
int				handle_signals_through_termios(t_var_data *var_data);
int				restore_tty(t_var_data *var_data);
>>>>>>> upstream/master

/* ************************************************************************** */
/*                                      error_and_free                        */
/* ************************************************************************** */

int				prompt_error_checks(t_token_node *first_node, \
					t_error_checks *error_checks);
int				ft_print_error(char *string);
void			*ft_print_error_null(char *string);
void			free_lexer(t_token_node *first_node_lexer);
void			big_free(t_var_data *var_data, char *prompt);

/* ************************************************************************** */
/*                                      cli                                   */
/* ************************************************************************** */

int				ms_command_line_inteface(t_var_data *var_data);
<<<<<<< HEAD
int				handle_pipes(t_var_data *var_data);
int				traverse_ast(t_ast_node *tmp_node, t_var_data *var_data, \
					int error_flow);
=======
>>>>>>> upstream/master
int				execute_logic(t_var_data *var_data);

/* ************************************************************************** */
/*                                      lexer                                 */
/* ************************************************************************** */

t_token_node	*tokenizer(char *prompt, t_var_data *var_data, \
					t_token_node *first_nd, int flow_check);
char			*redirect_handler(char *prompt, size_t *index, \
					t_var_data *var_data);
<<<<<<< HEAD
=======
int				fill_redirect_token(char *prompt, char *token, \
					int space_counter, t_var_data *var_data);
>>>>>>> upstream/master
int				single_quotes(char *prompt, size_t *index, \
					char **tokenized_string);
int				double_quotes(char *prompt, size_t *index, \
					t_var_data *var_data, char **token_str);
char			*ft_get_key(char *prompt);
<<<<<<< HEAD
int				ft_strtok(char *prompt, t_var_data *var_data, \
					char **token, size_t *i);
=======
int				ft_strtok(char *prompt, t_var_data *var_data, char **token, \
					size_t *i);
>>>>>>> upstream/master
t_token_node	*create_node(char *tokenized_str, t_token_node *prev_node, \
					t_token_node *current_node, t_var_data *var_data);
size_t			check_if_join_args(t_var_data *var_data, char *prompt, \
					char *tmp_str, t_token_node *current_node);
<<<<<<< HEAD
int				exception(char *prompt, size_t *index, \
					t_var_data *var_data, char **token);
int				no_quotes_arg(char *prompt, size_t *index, \
					char ***envvar, char **token);
=======
int				exception(char *prompt, size_t *index, t_var_data *var_data, \
					char **token);
int				no_quotes_arg(char *prompt, size_t *index, char ***envvar, \
					char **token);
>>>>>>> upstream/master
int				fill_token_expd_vars(char *prompt, char *token_string, \
					char *key, char ***envvar);
int				check_single_dollar(char *prompt, size_t *index, char **token);
int				count_total_strlen(char *prompt, t_var_data *var_data, \
					char *key, int *len_expanded_var);
int				count_key(char **key, char *prompt, int *i, int *j);
char			*expand_error_code_variable(char *argument, char *error_code, \
					int i, int j);
int				expand_error_codes(t_var_data *var_data);

/* ************************************************************************** */
/*                                      parser                                */
/* ************************************************************************** */

int				parser(t_ast_node **first_ast_node, \
					t_token_node *first_token_node, t_var_data *var_data);
t_ast_redir		*add_redirect(t_token_node **curr_token_node, \
					t_ast_redir *curr_redir_node, \
					t_ast_redir *first_redir_node, t_var_data *var_data);
t_ast_node		*create_ast_node(t_ast_node *prev_ast_node, \
					t_token_node **curr_token_node,	t_var_data *var_data);

/* ************************************************************************** */
/*                                      executor                              */
/* ************************************************************************** */

int				execute_builtin_or_binary(t_var_data *var_data, \
					t_ast_node *ast_node);
int				check_if_redir(t_var_data *var_data, t_ast_redir *redirect, \
					int error_check, char *command);
int				handle_here_doc(t_var_data *var_data, char *filename);
int				check_if_builtin(t_ast_node *ast_node);
int				run_builtins_with_output(t_var_data *var_data, \
					t_ast_node *ast_node);
int				run_builtins_without_output(t_var_data *var_data, \
					t_ast_node *ast_node);
int				ms_echo(t_var_data *var_data, t_ast_node *ast_node);
int				ms_env(t_var_data *var_data, t_ast_node *ast_node);
int				ms_export(t_var_data *var_data, t_ast_node *ast_node);
int				ms_unset(t_var_data *var_data, t_ast_node *ast_node);
void			ms_pwd(t_var_data *var_data);
int				ms_cd(t_var_data *var_data, t_ast_node *ast_node);
int				restore_fds(t_var_data *var_data);
int				check_pipe(t_var_data *var_data, t_ast_node *curr_node_pipe, \
					int *pipe_fd);
<<<<<<< HEAD
=======
char			*create_path_or_envp(char *directory_path, char *command, \
					char *separator);
char			*check_and_create_path(t_var_data *var_data, char *command);
char			**add_cmd_to_argarray(char **args, char *command);
char			**envvardict_to_envvararray(char ***envvar);
int				tmp_argarray_error_checks(char **tmp_arg_array,	\
					char **envvar_array, char *path_bin);
int				set_fds_and_continue_parent(t_var_data *var_data, \
					t_ast_node *ast_node, int pipe_fd[2]);
void			free_path_and_arrays(char *path_bin, char **envvar_array, \
					char **tmp_array);
>>>>>>> upstream/master

/* ************************************************************************** */
/*                                      sighandler                           */
/* ************************************************************************** */

int				sighandler(t_var_data *var_data, int mode);
int				restore_tty(t_var_data *var_data);
int				homemade_getpid(void);

#endif
