
#include "pipex.h"

char	*ft_strstr(char *str, char *to_find)
{
	int	i;
	int	j;
	int	ans;

	i = 0;
	if (to_find[0] == '\0')
	{
		return (str);
	}
	while (str[i] != '\0')
	{
		j = 0;
		ans = i;
		while (str[i] == to_find[j] && to_find[j] != '\0' && str[i] != '\0')
		{
			i++;
			j++;
		}
		if (to_find[j] == '\0')
		{
			return (&str[ans]);
		}
		i = ans + 1;
	}
	return (0);
}
static size_t	getalloc(unsigned int start, char const *s, int *b, size_t len)
{
	size_t	alloc;

	if (start >= ft_strlen(s))
	{
		alloc = 0;
		*b = 0;
	}
	else if (ft_strlen(s) >= (len + start))
		alloc = len;
	else
		alloc = ft_strlen(s) - start;
	return (alloc);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char		*a;
	size_t		n;
	char		*r;
	size_t		alloc;
	int			b;

	b = 1;
	alloc = getalloc(start, s, &b, len);
	r = (char *)s;
	n = 0;
	a = (char *)malloc(alloc + 1);
	if (!a)
	{
		return (0);
	}
	while (b && r[start + n] && n < len)
	{
		a[n] = r[start + n];
		n++;
	}
	a[n] = '\0';
	return (a);
}
size_t	ft_strlen(const char *str)
{
	size_t		a;

	a = 0;
	while (str[a] != '\0')
	{
		a++;
	}
	return (a);
}