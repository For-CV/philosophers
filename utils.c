#include "philosophers.h"

void    *ft_calloc(size_t nmemb, size_t size)
{
	void	*result;
	size_t  bytes;
	size_t	i;

	if (nmemb == 0 || size == 0)
	{
		result = malloc(0);
		return (result);
	}
	if (nmemb && nmemb > SIZE_MAX / size)
		return (NULL);
		bytes = nmemb * size;
	result = malloc(bytes);
	if (!result)
		return (NULL);
	i = 0;
	while (i < (bytes))
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

void	ft_free_list(t_philo **list)
{
	t_philo	*philo_d;
	t_philo	*start;
	t_philo	*next;

	if (!list || !*list)
		return ;
	philo_d = *list;
	start = *list;
	free(philo_d->forks);
	free(philo_d->dead_m);
	while  (philo_d)
	{
		next = philo_d->next;
		free(philo_d);
		philo_d = NULL;
		philo_d = next;
		if (philo_d == start)
			return ;
	}
	return ;
}

void	ft_set_dead_m(t_philo *philo_d, int dead)
{
	pthread_mutex_lock(philo_d->dead_m);
	*(philo_d->dead) = dead;
	pthread_mutex_unlock(philo_d->dead_m);
}
