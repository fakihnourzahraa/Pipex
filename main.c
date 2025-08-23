
#include "pipex.h"
#include <string.h>
char	*ft_strdup(const char *src)
{
	int		i;
	char	*new;

	i = 0;
	new = (char *)malloc(sizeof(char) * ft_strlen(src) + 1);
	if (!new)
		return (0);
	while (src[i])
	{
		new[i] = src[i];
		i++;
	}
	new[i] = '\0';
	return (new);
}
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
		}
		i++;
	}
	return (ans);
}
//create a function to free stuff here
// char	*get_path(t_pipe *a, char *name)
// {
// 	char	**dirs;
// 	char	*path;

// 	name = ft_split(name, ' ')[0];
// 	dirs = a->dirs;
// 	while(*dirs)
// 	{
// 		path = ft_strjoin(*dirs, "/");
// 		path = ft_strjoin(path, name);
// 		if (access(path, R_OK) != -1)
// 			return (path);
// 		dirs++;
// 	}
// 	return (NULL);
// }
char *get_path(t_pipe *a, char *name)
{
    char **dirs;
    char *path;
    char *temp_path;
    char **cmd_args;
    char *cmd_name;

    if (!name)
        return (NULL);

    // Split the command to get just the command name (first part)
    cmd_args = ft_split(name, ' ');
    if (!cmd_args || !cmd_args[0])
    {
        if (cmd_args)
            free_d(cmd_args);
        return (NULL);
    }

    cmd_name = cmd_args[0];  // Use first part as command name
    dirs = a->dirs;
    
    if (!dirs)
    {
        free_d(cmd_args);
        return (NULL);
    }
    
    while (*dirs)
    {
        temp_path = ft_strjoin(*dirs, "/");
        if (!temp_path)
        {
            free_d(cmd_args);
            return (NULL);
        }
        path = ft_strjoin(temp_path, cmd_name);
        free(temp_path);
        
        if (!path)
        {
            free_d(cmd_args);
            return (NULL);
        }
        
        if (access(path, X_OK) == 0)
        {
            free_d(cmd_args);
            return (path);
        }
        free(path);
        dirs++;
    }
    
    free_d(cmd_args);
    return (NULL);
}

void	cleanup(t_pipe *a, char *p, char **d)
{
	int	i;

	i = 0;
	free(a->cmd1_p);
	free(a->cmd2_p);
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
void	child1(t_pipe *a, char **argv)
{
	char 	*p;
	char	**d;

	if (a->pid1 == 0)
	{
		if (dup2(a->fd1, 0) == -1)
			exit(1);
		if (dup2(a->pfd[1], 1) == -1)
			exit(1);
		close(a->pfd[0]);
		close (a->fd1);
		close (a->fd2);
		close(a->pfd[1]);
		p = get_path(a, ft_strdup(argv[2]));
		if (!p)
			exit(127);
		d = ft_split(a->cmd1_p, ' ');
		if (execve(p, d, a->e) == -1)
		{
			free(p);
			free(d);
			exit(126);
		}
		free(d);
		free(p);
	}
}
void	child2(t_pipe *a, char **argv)
{
	char 	*p;
	char	**d;

	if (a->pid2 == 0)
	{
		if (dup2(a->pfd[0], 0) == -1)
			exit(1);
		if (dup2(a->fd2, 1) == -1)
			exit(126);
		close(a->pfd[0]);
		close (a->fd1);
		close (a->fd2);
		close(a->pfd[1]);
		p = get_path(a, ft_strdup(argv[3]));
		if (!p)
			exit(1);
		d = ft_split(a->cmd2_p, ' ');
		if (execve(p, d, a->e) == -1)
		{
			free(p);
			free(d);
			exit (126);
		}
		free(d);
		free(p);
	}
}

void	execute(t_pipe *a, char **argv)
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
	child1(a, argv);
	if (a->pid1 > 0)
		wait(s);
	a->pid2 = fork();
	if (a->pid2 == -1)
	{
		cleanup(a, NULL, NULL);
		exit(1);
	}
	child2(a, argv);
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
	execute(a, argv);
	// close(a->pfd[1]);
	// close(a->pfd[0]);
	cleanup(a, NULL, NULL);
    return (0);
}