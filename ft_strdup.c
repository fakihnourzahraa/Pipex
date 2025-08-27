/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfakih <nfakih@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 14:25:06 by nfakih            #+#    #+#             */
/*   Updated: 2025/05/24 14:25:06 by nfakih           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*ft_strdup(const char *s)
{
	int		size;
	char	*a;
	int		i;

	i = 0;
	if (!s)
		return (NULL);
	size = ft_strlen((char *)s);
	a = (char *)malloc(size + 1);
	if (!a)
	{
		return (0);
	}
	while (s[i])
	{
		a[i] = s[i];
		i++;
	}
	a[i] = '\0';
	return (a);
}
