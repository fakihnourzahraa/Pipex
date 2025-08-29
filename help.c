/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfakih <nfakih@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 15:54:24 by nfakih            #+#    #+#             */
/*   Updated: 2025/08/27 22:34:26 by nfakih           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	free_d(char	**a)
{
	int	i;

	i = 0;
	while (a[i])
	{
		free (a[i]);
		i++;
	}
	free(a);
}

char	*find_path(char **envp)
{
	int		i;
	char	*ans;

	i = 0;
	ans = NULL;
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], "PATH=", 5))
		{
			ans = ft_substr(envp[i], 5, (ft_strlen(envp[i]) - 5));
			break ;
		}
		i++;
	}
	return (ans);
}

void	free_twod(char **g)
{
	int	i;

	i = 0;
	if (!g)
		return ;
	while (g[i])
	{
		free(g[i]);
		i++;
	}
	free(g);
}

static char	*find_cmd(char **dirs, char *command)
{
	char	*path;
	char	*p;
	int		i;

	i = -1;
	while (dirs[++i])
	{
		p = ft_strjoin(dirs[i], "/");
		path = ft_strjoin(p, command);
		free(p);
		if (access(path, R_OK) != -1)
			return (path);
		free(path);
	}
	return (NULL);
}

void	get_path(t_pipe *a, char *name, int c)
{
	char	*path;
	char	*g;
	char	**splitting;
	int		i;

	if (!name)
		return ;
	splitting = ft_split(name, ' ');
	g = splitting[0];
	path = find_cmd(a->dirs, g);
	if (path)
	{
		if (c == 1)
			a->cmd1_p = ft_strdup(path);
		else
			a->cmd2_p = ft_strdup(path);
		free(path);
	}
	i = -1;
	while (splitting[++i])
		free(splitting[i]);
	free(splitting);
}
