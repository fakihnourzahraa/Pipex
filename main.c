
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
		if (ft_strstr(envp[i], "PATH="))
		{
			ans = ft_substr(envp[i], 5, (ft_strlen(envp[i]) - 5));
			break;
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
	char	**dirs;
	char	*path;
	char	**p;
	char	*g;
	char	**splitting;

	splitting = ft_split(name, ' ');
	g = splitting[0];
	free_twod(splitting);
	name = g;
	dirs = a->dirs;
	while(*dirs)
	{
		path = ft_strjoin(*dirs, "/");
		p = ft_strjoin(path, name);
		if (access(path, R_OK) != -1)
		{
			if (c == 1)
				a->cmd1_p = *dirs;
			else
				a->cmd2_p = *dirs;
			free(path);
			break ;
		}
		free(path);
		dirs++;
	}
	free_twod(dirs);
}

void	cleanup(t_pipe *a, char *p, char **d)
{
	int	i;

	i = 0;
	free(a->in);
	free(a->out);
	free(a->cmd1_p);
	free(a->cmd2_p);
	free(a->c1);
	free(a->c2);
	if (p)
		free(p);
	i = -1;
	if (d)
		free_twod(d);
	free(a);
}
int	child1(t_pipe *a, char ***e)
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
			return(exit(126), 0);
	}
	return (1);
}
int	child2(t_pipe *a, char ***e)
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
			return(/*cleanup(a, p, d)*/exit(126), 0);
	}
	return (1);
}

void	execute(t_pipe *a, char ***e)
{
	int *s;

	s = NULL;
	pipe(a->pfd);
	a->pid1 = fork();
	if (a->pid1 == -1)
	{
		// cleanup(a, NULL, NULL);
		exit(1);
	}
	child1(a, e);
	if (a->pid1 > 0)
		wait(s);
	a->pid2 = fork();
	if (a->pid2 == -1)
	{
		// cleanup(a, NULL, NULL);
		exit(1);
	}
	child2(a, e);
	if (a->pid2 > 0)
		wait(s);
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
	a->dirs = ft_split(find_path(e), ':');
	a->fd1 = open(a->in, O_RDONLY);
	a->fd2 = open(a->out, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	get_path(a, a->c1, 1);
	get_path(a, a->c2, 2);
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
	execute(a, envp);
	close(a->pfd[0]);
	close(a->pfd[1]);
	cleanup(a, NULL, NULL);
    return (0);
}