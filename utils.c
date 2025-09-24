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