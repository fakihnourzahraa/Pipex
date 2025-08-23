
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
	int		j;
	char	*ans;

	i = 0;
	j = 0;
	ans = NULL;
	while (envp[i])
	{
		if (ft_strstr(envp[i], "PATH="))
		{
			ans = ft_substr(envp[i], 5, (ft_strlen(envp[i]) - 5));
		}
		j++;
	}
	return (ans);
}
//create a function to free stuff here
char	*get_path(t_pipe *a, char *name)
{
	char	**dirs;
	char	*path;

	name = ft_split(name, ' ')[0];
	dirs = a->dirs;
	while(*dirs)
	{
		path = ft_strjoin(*dirs, "/");
		path = ft_strjoin(path, name);
		if (access(path, R_OK) != -1)
			return (path);
		dirs++;
	}
	return (NULL);
}
void	cleanup(t_pipe *a, char *p, char **d)
{
	int	i;

	i = 0;
	free(a->cmd1_p);
	free(a->cmd2_p);
	free(a->in);
	free(a->out);
	while(a->dirs[i])
	{
		free(a->dirs[i]);
		i++;
	}
	free(a->dirs);
	if (p)
		free(p);
	i = -1;
	if (d)
	{
		while (d[++i])
			free(d[i]);
		free(d);
	}
	free(a);
}
int	child1(t_pipe *a)
{
	char 	*p;
	char	**d;

	if (a->pid1 == 0)
	{
		if (dup2(a->fd1, 0) == -1)
			return (cleanup(a, NULL, NULL), 0);
		close (a->fd1);
		if (dup2(a->pfd[0], 1) == -1)
			return(cleanup(a, NULL, NULL), exit(126), 0);
		close(a->pfd[0]);
		p = get_path(a, a->cmd1_p);
		if (!p)
			return(cleanup(a, NULL, NULL), exit(1), 0);
		d = ft_split(a->cmd1_p, ' ');
		if (execve(p, d, a->e) == -1)
			return(cleanup(a, p, d), exit(126), 0);
	}
	free(p);
	return (1);
}
int	child2(t_pipe *a)
{
	char 	*p;
	char	**d;

	if (a->pid1 == 0)
	{
		if (dup2(a->pfd[0], 0) == -1)
			return (cleanup(a, NULL, NULL), 0);
		close(a->pfd[0]);
		if (dup2(a->fd2, 1) == -1)
			return(cleanup(a, NULL, NULL), exit(126), 0);
		close (a->fd2);
		p = get_path(a, a->cmd2_p);
		if (!p)
			return(cleanup(a, NULL, NULL), exit(1), 0);
		d = ft_split(a->cmd2_p, ' ');
		if (execve(p, d, a->e) == -1)
			return(cleanup(a, p, d), exit(126), 0);
	}
	free(p);
	return (1);
}

void	execute(t_pipe *a)
{
	int *s;

	s = NULL;
	pipe(a->pfd);
	a->pid1 = fork();
	if (a->pid1 == -1)
	{
		cleanup(a, NULL, NULL);
		exit(1);
	}
	child1(a);
	if (a->pid1 > 0)
		wait(s);
	a->pid2 = fork();
	if (a->pid2 == -1)
	{
		cleanup(a, NULL, NULL);
		exit(1);
	}
	child2(a);
	if (a->pid2 > 0)
		wait(s);
} 
//pids: 0 is child positive is parent -1 is error
void	intialize_pipe(t_pipe *a, char **argv, char **e)
{
	a->in = argv[1];
	a->out = argv[4];
	a->e = e;
	a->dirs = ft_split(find_path(e), ':');
	a->fd1 = open(a->in, O_RDONLY);
	a->fd2 = open(a->out, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	a->cmd1_p = get_path(a, argv[2]);
	a->cmd2_p = get_path(a, argv[3]);
}
int main(int argc, char **argv, char **envp)
{
	t_pipe	*a;

	if (argc != 5)
		exit(2);
	a = malloc(sizeof(t_pipe));
	if (!a)
		return (0);
	intialize_pipe(a, argv, envp);
	execute(a);
	close(a->pfd[1]);
	close(a->pfd[1]);
	cleanup(a, NULL, NULL);
    return (0);
}