
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
int	src_p(char **argv, char *dirs)
{

}
int	pipex(char **argv, char **e)
{
	char	*dirs;
	char	**in;
	char	**cmd1;
	char	*cmd2;

	in = ft_split(argv, ' ');
	if (!(src_p(in[0], e) && src_p(in[1], e) && src_p(in[2], e) ||
		src_p(in[3], e)))
		return (0);
	dirs = ft_split(find_path(e), ':');

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