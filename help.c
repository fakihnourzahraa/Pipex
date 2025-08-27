/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 15:54:24 by nfakih            #+#    #+#             */
/*   Updated: 2025/08/27 22:26:10 by yitani           ###   ########.fr       */
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

static char	*find_command_in_path(char **dirs, char *command)
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
	path = find_command_in_path(a->dirs, g);
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

void	cleanup(t_pipe *a, char *p, char **d)
{
	free(a->in);
	free(a->out);
	int i = 0;
	while (a->dirs[i])
	{
		free(a->dirs[i]);
		i++;
	}
	free(a->dirs);
	if (a->cmd1_p)
		free(a->cmd1_p);
	if (a->cmd2_p)
		free(a->cmd2_p);
	free(a->c1);
	free(a->c2);
	if (p)
		free(p);
	if (d)
		free_twod(d);
	free(a);
}

int	child1(t_pipe *a, char **e)
{
	char	**d;

	if (a->pid1 == 0)
	{
		if (dup2(a->fd1, 0) == -1)
		{
			cleanup(a, NULL, NULL);
			exit(1);
		}
		close (a->fd1);
		if (dup2(a->pfd[1], 1) == -1)
			return (0);
		close(a->pfd[1]);
		close(a->pfd[0]);
		d = ft_split(a->c1, ' ');
		if (!a->cmd1_p)
			return(cleanup(a, NULL, d), exit(127), 0);
		if (execve(a->cmd1_p, d, e) == -1)
			return(cleanup(a, NULL, d), exit(127), 0);
	}
	return (1);
}

int	child2(t_pipe *a, char **e)
{
	char	**d;

	if (a->pid2 == 0)
	{
		if (dup2(a->pfd[0], 0) == -1)
			return(cleanup(a, NULL, NULL), exit(1), 1);
		close(a->pfd[0]);
		close(a->pfd[1]);
		if (dup2(a->fd2, 1) == -1)
			return(cleanup(a, NULL, NULL), exit(1), 0);
		close (a->fd2);
		d = ft_split(a->c2, ' ');
		if (!a->cmd2_p)
			return(cleanup(a, NULL, d), exit(127), 0);
		if (execve(a->cmd2_p, d, e) == -1)
			return(cleanup(a, NULL, d), exit(127), 0);
	}
	return (1);
}
