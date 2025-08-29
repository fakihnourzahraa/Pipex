/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfakih <nfakih@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 22:31:57 by nfakih            #+#    #+#             */
/*   Updated: 2025/08/27 22:31:59 by nfakih           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <sys/wait.h>

typedef struct s_pipe
{
	char	*in;
	char	*out;
	char	*cmd1_p;
	char	*cmd2_p;
	int		fd1;
	int		fd2;
	pid_t	pid1;
	pid_t	pid2;
	int		pfd[2];
	char	**e;
	char	*c1;
	char	*c2;
	char	**dirs;
}			t_pipe;

char	**ft_split(const char *str, char c);
size_t	ft_strlen(const char *str);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strstr(char *str, char *to_find);
char	*ft_strdup(const char *s);
int		ft_strncmp(const char *s1, const char *s2, size_t n);

void	free_d(char	**a);
char	*find_path(char **envp);
void	free_twod(char **g);
void	get_path(t_pipe *a, char *name, int c);

void	cleanup(t_pipe *a, char *p, char **d);
int		child1(t_pipe *a, char **e);
int		child2(t_pipe *a, char **e);

void	cleanup(t_pipe *a, char *p, char **d);
int		child1(t_pipe *a, char **e);
int		child2(t_pipe *a, char **e);

#endif