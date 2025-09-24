#include "philosophers.h"

void    *ft_calloc(size_t nmemb, size_t size)
{
	void	*result;
	int	i;

	if (nmemb == 0 || size == 0)
	{
		result = malloc(0);
		return (result);
	}
	if (nmemb && size > SIZE_MAX / size)
		return (NULL);
	result = malloc(nmemb * size);
	if (!result)
		return (NULL);
	i = 0;
	while (i < (nmemb * size))
	{
		((char *)result)[i] = '\0';
		i++;
	}
	return (result);
}

int	ft_isspace(int	c)
{
	if (c == 32 || (c >= 9 && c <= 13))
		return (1);
	return (0);
}

int	ft_strlen(char *s)
{
	int	i;

	i = 0;
	if (!s)
		return (0);
	while (s[i])
		i++;
	return (i);
}

static int	ft_jump_and_sign(char *s)
{
	int	i;
	int	sign;

	sign = 1;
	i = 0;
	while (ft_isspace(s[i]))
		i++;
	while (s[i] == '-' || s[i] == '+')
	{
		if (s[i] == '-')
			sign = -sign;
		i++;
	}
	return (i * sign);
}

int	ft_atoi(char *s)
{
	int	i;
	int	r;

	if (!s)
		return (-1);
	i = ft_jump_and_sign(s);
	if (i < 0)
		return (0);
	r = 0;
	while (s[i] >= '0' && s[i] <= '9')
	{
		r = r * 10 + (s[i] - '0');
		i++;
	}
	// printf("len = %d, i = %d\n", ft_strlen(s), i);
	if (i < ft_strlen(s))
		return (-1);
	return (r);
}