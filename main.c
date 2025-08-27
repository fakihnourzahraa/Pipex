/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 15:54:11 by nfakih            #+#    #+#             */
/*   Updated: 2025/08/27 21:33:31 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void    execute(t_pipe *a, char **e)
{
    int status1, status2;
    int final_exit = 0;

    if (pipe(a->pfd) == -1)
	{
		cleanup(a, NULL, NULL);
		exit(1);
	}
    a->pid1 = fork();
    if (a->pid1 == -1)
    {
		close(a->pfd[0]);
        close(a->pfd[1]);
        cleanup(a, NULL, NULL);
        exit(1);
    }
    child1(a, e);
    a->pid2 = fork();
    if (a->pid2 == -1)
    {
		close(a->pfd[0]);
        close(a->pfd[1]);
        cleanup(a, NULL, NULL);
        exit(1);
    }
    child2(a, e);
    close(a->pfd[0]);
    close(a->pfd[1]);
    waitpid(a->pid1, &status1, 0);
    waitpid(a->pid2, &status2, 0);

    if (WIFEXITED(status2))
        final_exit = WEXITSTATUS(status2);
    else if (WIFSIGNALED(status2))
        final_exit = 128 + WTERMSIG(status2);

    cleanup(a, NULL, NULL);
    exit(final_exit);
}

//pids: 0 is child positive is parent -1 is error
void	intialize_pipe(t_pipe *a, char **argv, char **e)
{
	char	*m;

	m = find_path(e);
	a->in = ft_strdup(argv[1]);
	a->out = ft_strdup(argv[4]);
	a->c1 = ft_strdup(argv[2]);
	a->c2 = ft_strdup(argv[3]);
	a->dirs = ft_split(m, ':');
	free(m);
	get_path(a, a->c1, 1);
	get_path(a, a->c2, 2);
	a->fd1 = open(a->in, O_RDONLY);
	a->fd2 = open(a->out, O_WRONLY | O_CREAT | O_TRUNC, 0644);
}
int main(int argc, char **argv, char **envp)
{
	t_pipe	*a;

	if (argc != 5)
		exit(2);
	a = malloc(sizeof(t_pipe));
	if (!a)
		return (0);
	a->dirs = NULL;
	a->cmd1_p = NULL;
	a->cmd2_p = NULL;
	a->in = NULL;
	a->out = NULL;
	a->c1 = NULL;
	a->c2 = NULL;
	intialize_pipe(a, argv, envp);
	execute(a, envp);
	// close(a->pfd[0]);
	// close(a->pfd[1]);
	cleanup(a, NULL, NULL);
    return (0);
}