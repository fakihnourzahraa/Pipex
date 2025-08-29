/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help_two.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfakih <nfakih@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 22:29:32 by nfakih            #+#    #+#             */
/*   Updated: 2025/08/27 22:30:53 by nfakih           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	cleanup(t_pipe *a, char *p, char **d)
{
	int	i;

	i = 0;
	free(a->in);
	free(a->out);
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
			return (cleanup(a, NULL, d), exit(127), 0);
		if (execve(a->cmd1_p, d, e) == -1)
			return (cleanup(a, NULL, d), exit(127), 0);
	}
	return (1);
}

int	child2(t_pipe *a, char **e)
{
	char	**d;

	if (a->pid2 == 0)
	{
		if (dup2(a->pfd[0], 0) == -1)
			return (cleanup(a, NULL, NULL), exit(1), 1);
		close(a->pfd[0]);
		close(a->pfd[1]);
		if (dup2(a->fd2, 1) == -1)
			return (cleanup(a, NULL, NULL), exit(1), 0);
		close (a->fd2);
		d = ft_split(a->c2, ' ');
		if (!a->cmd2_p)
			return (cleanup(a, NULL, d), exit(127), 0);
		if (execve(a->cmd2_p, d, e) == -1)
			return (cleanup(a, NULL, d), exit(127), 0);
	}
	return (1);
}
