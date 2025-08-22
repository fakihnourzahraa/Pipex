
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
	if (!ans)
		return (free(),exit(127), NULL);
	return (ans);
}
//create a function to free stuff here
char	*get_path(char **dirs, char *name)
{
	char	*path;

	
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

int	pipex(char **argv, char **e)
{
	char	**dirs;
	char	**in;
	char	**cmd1;
	char	*cmd2;

	in = ft_split(argv, ' ');
	if (!(src_p(in[0], e) && src_p(in[1], e) && src_p(in[2], e) ||
		src_p(in[3], e)))
		return (0);
	dirs = ft_split(find_path(e), ':');
	get_path(dirs, in[0]);
	free(dirs);
	free_d(in);

}
//in[0] = infile
//in[1] = cmd1
//in[2] = cmd2
//in[3] = outfile
int main(int argv, char **argc, char **envp)
{
	if (argv != 4)
		return (0);
	pipex(argc, envp);
	//freeing

    return 0;
}