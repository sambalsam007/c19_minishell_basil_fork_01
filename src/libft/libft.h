/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 15:24:49 by bclaeys           #+#    #+#             */
/*   Updated: 2024/11/07 17:40:00 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stdarg.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdint.h>
# include <stdio.h>
# include <limits.h>

/******************************************************************************/
/*                                 libft                                      */
/******************************************************************************/

/* typedefs */

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}					t_list;

/* These are ft_functions that are recoded (part 1 of the PDF) to work 
 * exactly like their equivalents in libc. No other functions where used. */

int		ft_isprint(int c);
int		ft_isdigit(int c);
int		ft_isascii(int c);
int		ft_isalpha(int c);
int		ft_isalnum(int c);
int		ft_toupper(int c);
int		ft_tolower(int c);
int		ft_atoi(const char *str);
size_t	ft_strlen(const char *str);
char	*ft_strrchr(const char *s, int c);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strchr(const char *s, int c);
char	*ft_strnstr(const char *big, const char *little, size_t len);
size_t	ft_strlcat(char *dst, const char *src, size_t size);
size_t	ft_strlcpy(char *dst, const char *src, size_t size);
int		ft_memcmp(const void *s1, const void *s2, size_t n);
void	*ft_memchr(const void *s, int c, size_t n);
void	*ft_memset(void *s, int c, size_t n);
void	ft_bzero(void *s, size_t n);
void	*ft_memcpy(void *dest, const void *src, size_t n);
void	*ft_memmove(void *dest, const void *src, size_t n);

/* These are ft_functions that are recoded (part 1 of the PDF) to work exactly
like their equivalents in libc. Only malloc() was used. */

void	*ft_calloc(size_t nmemb, size_t size);
char	*ft_strdup(const char *s);

/* These are ft_functions that are recoded (part 1 of the PDF) to work almost
like their equivalents in libc. */

char	*ft_itoa(int n);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strtrim(char const *s1, char const *set);
char	**ft_split(char const *s, char c);
char	*ft_strmapi(char const *s, char (*f)(unsigned int, char));
void	ft_striteri(char *s, void (*f)(unsigned int, char*));
void	ft_putchar_fd(char c, int fd);
void	ft_putstr_fd(char *s, int fd);
void	ft_putendl_fd(char *s, int fd);
void	ft_putnbr_fd(int n, int fd);

/* Bonus functions */

t_list	*ft_lstnew(void *content);
void	ft_lstadd_front(t_list **lst, t_list *new_list);
int		ft_lstsize(t_list *lst);
t_list	*ft_lstlast(t_list *lst);
void	ft_lstadd_back(t_list **lst, t_list *new_list);
void	ft_lstdelone(t_list *lst, void (*del)(void *));
void	ft_lstclear(t_list **lst, void (*del)(void *));
void	ft_lstiter(t_list *lst, void (*f)(void *));
t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));

/******************************************************************************/
/*                               ft_printf                                    */
/******************************************************************************/

void	ft_format_and_count(char s, va_list ap, size_t *count);
size_t	ftpr_putchar(char c);
size_t	ftpr_puthex(size_t h, char c);
size_t	ftpr_putint(int n);
size_t	ftpr_putstr(char *s);
size_t	ftpr_putptr(size_t p);
size_t	ftpr_putunsdec(unsigned int d);
int		ft_printf(const char *s, ...);

/******************************************************************************/
/*                               get_next_line                                */
/******************************************************************************/

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 9 
# endif

void	ft_bzero(void *s, size_t n);
void	*ft_calloc(size_t nmemb, size_t size);
char	*get_next_line(int fd);
char	*create_result(int i, char *s);
int		ft_extend(char **line, int *l_buf_mul);
void	ft_reset_s(char *s, int j);
char	*ft_write_line(char *s, char *line, int bytes_returned, int j);
int		ft_s_eof_check(char *s, char *line, int bytes_returned);
void	ft_set_to_zero(int *i, int *j, int *k);

/******************************************************************************/
/*                               extra fcts                                   */
/******************************************************************************/

int		ft_iswhitespace(int c);
char	***ft_addto_dic(char *key, char *value, char ***dict);
void	ft_print_dict(char ***dict, char *separator);
char	***ft_delete_from_dict(char *key, char ***dict);
char	*ft_get_value(char *key, char ***dict);
void	ft_delete_dict(char ***dict);
char	***ft_create_dict(char **list, char separator);

#endif
