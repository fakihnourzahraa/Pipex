/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfakih <nfakih@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 15:54:24 by nfakih            #+#    #+#             */
/*   Updated: 2025/08/27 19:48:40 by nfakih           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	free_d(char	**a)
{
	int i;

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
//create a function to free stuff here
void	get_path(t_pipe *a, char *name, int c)
{
	char	*path;
	char	*p;
	char	*g;
	char	**splitting;
	int		i;

	if (!name)
	{
		a->cmd1_p = malloc(sizeof(char) * 1);
		a->cmd2_p = malloc(sizeof(char) * 1);
		return ;
	}
	splitting = ft_split(name, ' ');
	g = splitting[0];
	i = 0;
	while(a->dirs[i])
	{
		p = ft_strjoin(a->dirs[i], "/");
		path = ft_strjoin(p, g);
		free(p);
		if (access(path, R_OK) != -1)
		{
			if (c == 1)
				a->cmd1_p = ft_strdup(a->dirs[i]);
			else
				a->cmd2_p = ft_strdup(a->dirs[i]);
			free(path);
			break ;
		}
		free(path);
	i++;
	}
	i = 0;
	while (splitting[i])
	{
		free(splitting[i]);
		i++;
	}
	free(splitting);
}
//assigns the paths for the commands

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
	free(a->cmd1_p);
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
			return (0);
		close (a->fd1);
		if (dup2(a->pfd[1], 1) == -1)
			return(exit(126), 0);
		close(a->pfd[1]);
		get_path(a, a->cmd1_p, 1);
		d = ft_split(a->cmd1_p, ' ');
		if (execve(a->cmd1_p, d, e) == -1)
			return(cleanup(a, NULL, d), exit(126), 0);
	}
	return (1);
}
int	child2(t_pipe *a, char **e)
{
	char	**d;

	if (a->pid2 == 0)
	{
		if (dup2(a->pfd[0], 0) == -1)
			return (0);
		close(a->pfd[0]);
		if (dup2(a->fd2, 1) == -1)
			return(exit(126), 0);
		close (a->fd2);
		get_path(a, a->cmd2_p, 2);
		d = ft_split(a->cmd2_p, ' ');
		if (execve(a->cmd2_p, d, e) == -1)
			return(cleanup(a, NULL, d), exit(126), 0);
	}
	return (1);
}