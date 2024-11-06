/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 11:26:32 by bclaeys           #+#    #+#             */
/*   Updated: 2024/11/04 13:33:15 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

static void	free_all(int j, char **ss)
{
	while (j >= 0)
	{
		free(ss[j]);
		j--;
	}
	free(ss);
}

static size_t	calc_len(char const *s, char c)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	if (!s[i])
		return (0);
	while (s[i])
	{
		while (s[i] == c)
			i++;
		if (s[i])
			len++;
		while (s[i] != c && s[i])
			i++;
	}
	return (len);
}

static int	word_len(char const *s, int start, char c)
{
	int	i;

	i = 0;
	while (s[start] != c && s[start] != 0)
	{
		i++;
		start++;
	}
	return (i);
}

static char	**make_table(char **ss, char const *s, char c)
{
	int	j;
	int	i;

	i = 0;
	j = 0;
	while (s[i])
	{
		while (s[i] == c && s[i])
			i++;
		if (s[i])
		{
			ss[j] = ft_substr(s, i, word_len(s, i, c));
			if (!ss[j])
			{
				free_all(j - 1, ss);
				return (NULL);
			}
			j++;
		}
		while (s[i] != c && s[i])
			i++;
	}
	ss[j] = NULL;
	return (ss);
}

char	**ft_split(char const *s, char c)
{
	char	**ss;

	if (s == 0)
		return (0);
	ss = malloc(sizeof(char *) * (calc_len(s, c) + 1));
	if (!ss)
		return (NULL);
	ss = make_table(ss, s, c);
	return (ss);
}
