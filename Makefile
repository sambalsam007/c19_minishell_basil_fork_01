# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/24 11:58:53 by bclaeys           #+#    #+#              #
#    Updated: 2024/11/11 11:35:01 by bclaeys          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME=minishell

MAIN_SRCS= minishell_main.c init_data.c
CLI_SRCS = command_line_inteface.c
EXECUTOR_SRCS =
LEXER_SRCS = tokenizer.c quote_handler.c redirect_handler.c
PARSER_SRCS =
SIGHANDLER_SRCS =
ERRORHANDLER_SRCS = error_checks.c big_free.c error_messages.c

CC=clang
CFLAGS= -Wall -Wextra -Werror -g
DEBUG_FLAGS= -fsanitize=address -fsanitize=undefined -fsanitize=bounds -fsanitize=null
INCLUDES = -I ./libf
INCFLAGS= -I src/libft -L src/libft -lft -lreadline
LIBFT=./src/libft/libft.a

SRCS = $(MAIN_SRCS) $(CLI_SRCS) $(EXECUTOR_SRCS) \
	$(LEXER_SRCS) $(PARSER_SRCS) $(SIGHANDLER_SRCS) $(ERRORHANDLER_SRCS)

SRC_DIRS = src/cli src/executor/builtins src/executor/exec_base \
		   src/executor/pipe src/executor/redir src/lexer src/main \
		   src/parser src/sighandlers src/error_handler
vpath %.c $(SRC_DIRS)  

OBJECTS = $(patsubst %.c,obj/%.o,$(SRCS))

all: $(LIBFT) obj $(NAME)
	@(make -q $(NAME) && echo "Everything up to date") || (make  build)

$(LIBFT):
	@make -s -C src/libft 	

$(NAME): $(OBJECTS) 
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) -o $(NAME) $(OBJECTS) $(INCFLAGS)
	@echo "\033[33mMaking compile_commands.json...\033[0m"
	@find . -type f -name "compile_commands.json" -delete
	@find ./obj/ -type f -name "*.json" | xargs sed -e '1s/^/[\n/' >> compile_commands.json
	@find ./src/libft/ -type f -name "*.json" | xargs sed -e '$$s/,$$/\n]/' >> compile_commands.json
	@find ./obj/ -type f -name "*.json" -delete
	@find ./src/libft/ -type f -name "*.json" -delete
	@echo "\033[33mminishell created\033[0m"

obj:
	@mkdir -p obj

obj/%.o: %.c
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) -MJ $@.json -c -o $@ $< $(INCLUDES)

clean:
	@rm -rf obj
	@if [ -d "obj" ]; then rmdir obj/; \
	fi
	@echo "\033[33mAll object files, dependency files, archive files and test files removed.\033[0m"

fclean: clean
	@rm -rf $(NAME)
	@make -s $@ -C src/libft
	@echo "\033[33mBinary removed.\033[0m"

re: fclean all

.PHONY: all clean fclean re obj 