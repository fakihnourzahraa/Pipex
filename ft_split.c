/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfakih <nfakih@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 09:28:26 by nfakih            #+#    #+#             */
/*   Updated: 2025/08/27 19:38:49 by nfakih           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	wordcount(char *str, char c)
{
	int	i;
	int	words;

	i = 0;
	words = 0;
	while (str && str[i])
	{
		if (str[i] != c)
		{
			words++;
			while (str[i] && str[i] != c)
				i++;
		}
		else
			i++;
	}
	return (words);
}

char	*extract(char *str, char c)
{
	int		i;
	char	*word;

	i = 0;
	while (str[i] && str[i] != c)
		i++;
	word = (char *)malloc((i + 1));
	if (!word)
		return (NULL);
	i = 0;
	while (str[i] && str[i] != c)
	{
		word[i] = str[i];
		i++;
	}
	word[i] = '\0';
	return (word);
}

static void	freeing(char **result, int j)
{
	while (j >= 0)
	{
		free(result[j]);
		j--;
	}
	free(result);
}

static char	**help(char *s, int i, char c, char **result)
{
	int	j;

	j = 0;
	while (s[i])
	{
		if (s[i] != c)
		{
			result[j] = extract(&s[i], c);
			if (result[j] == NULL)
			{
				freeing(result, j);
				return (NULL);
			}
			while (s[i] && s[i] != c)
				i++;
			j++;
		}
		else
			i++;
	}
	result[j] = NULL;
	return (result);
}

char	**ft_split(const char *str, char c)
{
	char	**result;
	char	*s;

	s = (char *)str;
	if (!str)
		return (NULL);
	result = (char **)malloc(sizeof(char *) * (wordcount(s, c) + 1));
	if (!result)
		return (NULL);
	return (help(s, 0, c, result));
}
