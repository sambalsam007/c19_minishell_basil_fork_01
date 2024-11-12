# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/22 11:15:35 by bclaeys           #+#    #+#              #
#    Updated: 2024/11/07 17:25:47 by bclaeys          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME=libft.a 
TESTFILE=test
CODEDIRS=.
INCDIRS=.
CFILES=ft_format.c ft_printf.c ftpr_putchar.c ftpr_puthex.c ftpr_putint.c \
	   ftpr_putstr.c ftpr_putptr.c ftpr_putunsdec.c get_next_line_utils.c \
	   get_next_line.c ft_isdigit.c ft_memmove.c ft_strlen.c ft_toupper.c \
	   ft_bzero.c ft_isprint.c ft_memset.c ft_strncmp.c ft_isalnum.c \
	   ft_memchr.c ft_strchr.c ft_strnstr.c ft_isalpha.c ft_memcmp.c \
	   ft_strlcat.c ft_strrchr.c ft_isascii.c ft_memcpy.c ft_strlcpy.c \
	   ft_tolower.c ft_atoi.c ft_calloc.c ft_strdup.c ft_substr.c ft_strjoin.c \
	   ft_strtrim.c ft_split.c ft_itoa.c ft_strmapi.c ft_striteri.c \
	   ft_putchar_fd.c ft_putendl_fd.c ft_putnbr_fd.c ft_putstr_fd.c \
	   ft_lstnew.c ft_lstadd_front.c ft_lstsize.c ft_lstlast.c \
	   ft_lstadd_back.c ft_lstdelone.c ft_lstclear.c ft_lstiter.c ft_lstmap.c \
	   ft_iswhitespace.c ft_addto_dict.c ft_get_value.c \
	   ft_print_dict.c ft_deletefrom_dict.c ft_delete_dict.c ft_create_dict.c

OPT=-O0
CC=clang
CFLAGS=-Wall -Wextra -Werror -g
OBJECTS=$(patsubst %.c,obj/%.o,$(CFILES))

all: obj $(NAME)
	
$(NAME): $(OBJECTS)
	@echo "Making libft..."
	@ar rcs $@ $^
	@echo "Making libft_compile_comands_json..."
	@find . -type f -name "*.o" -delete

obj:
	@mkdir -p obj

obj/%.o: %.c
	$(CC) $(CFLAGS) -MJ $@.json -c -o $@ $<

%.o:%.c
	$(CC) $(CFLAGS) -MJ $@.json -c -o $@ $<
	
clean:
	@rm -f $(OBJECTS) $(DEPFILES) $(TESTFILE)
	@rm -f obj/%.o.json
	@rmdir obj

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re test
