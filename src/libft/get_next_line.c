/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 15:02:33 by bclaeys           #+#    #+#             */
/*   Updated: 2024/08/05 14:41:07 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*get_next_line(int fd)
{
	static char	s[BUFFER_SIZE + 1];
	char		*line;
	char		*tmp;

	tmp = ft_calloc(BUFFER_SIZE + 1, sizeof(char));
	if (!tmp)
		return (NULL);
	if (read(fd, tmp, BUFFER_SIZE - BUFFER_SIZE))
	{
		ft_bzero(&s[0], BUFFER_SIZE + 1);
		free(tmp);
		return (NULL);
	}
	free(tmp);
	if (fd < 0 || fd == PATH_MAX)
		return (NULL);
	if (!s[0])
		ft_bzero(s, BUFFER_SIZE + 1);
	line = ft_calloc(BUFFER_SIZE + 1, sizeof(char));
	if (!line)
		return (NULL);
	line = ft_write_line(s, line, 0, fd);
	if (!line)
		return (NULL);
	return (line);
}

char	*ft_write_line(char *s, char *line, int bytes_read, int fd)
{
	int	i;
	int	j;
	int	l_buf_mul;

	ft_set_to_zero(&i, &j, &l_buf_mul);
	while (s[j] != '\n')
	{
		if (!s[j])
		{
			bytes_read = read(fd, &s[0], BUFFER_SIZE);
			if (bytes_read <= 0 && (line[0]))
				break ;
			if ((ft_s_eof_check(s, line, bytes_read)) == -1)
				return (NULL);
			j = 0;
		}
		if ((i == BUFFER_SIZE * l_buf_mul) && (!ft_extend(&line, &l_buf_mul)))
			return (NULL);
		if (s[j] == '\n' || (!s[j] && bytes_read < BUFFER_SIZE))
			break ;
		line[i++] = s[j++];
	}
	(void)((s[j] == '\n' || s[j] == '\0') && (line[i] = s[j]));
	ft_reset_s(s, j + 1);
	return (line);
}

int	ft_s_eof_check(char *s, char *line, int bytes_read)
{
	if (bytes_read < BUFFER_SIZE)
		ft_bzero(&s[bytes_read], BUFFER_SIZE - bytes_read);
	if (bytes_read <= 0)
	{
		free(line);
		return (-1);
	}
	return (0);
}

int	ft_extend(char **line, int *l_buf_mul)
{
	char	*new_line;
	int		i;

	i = 0;
	(*l_buf_mul)++;
	new_line = ft_calloc(BUFFER_SIZE + (BUFFER_SIZE * *l_buf_mul) + 1,
							sizeof(char));
	if (!new_line)
		return (0);
	while (line[0][i])
	{
		new_line[i] = line[0][i];
		i++;
	}
	free(*line);
	*line = new_line;
	return (1);
}

void	ft_reset_s(char *s, int j)
{
	int	i;

	i = 0;
	while (s[j])
	{
		s[i] = s[j];
		i++;
		j++;
	}
	ft_bzero(&s[i], BUFFER_SIZE - i);
}
/*
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

int	main(void)
{
	int		fd;
	char	*ptr;

	fd = open("example.txt", O_RDONLY);
	while ((ptr = get_next_line(fd)) != NULL)
	{
		printf("%s", ptr);
		free(ptr);
	}
}
*/
