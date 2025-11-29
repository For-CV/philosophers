#include "philo.h"

/* Imprime la muerte de un filósofo. */
static void	ft_print_dead(const t_philo *philo)
{
	long	current_t;

	current_t = ft_get_time();
	pthread_mutex_lock(philo->printer);
	if (current_t >= 0)
		printf("%ld ms %d died\n", current_t - philo->start_ms, philo->philo_id);
	else
		printf("%ld ms %d died\n", current_t, philo->philo_id);
	pthread_mutex_unlock(philo->printer);
}

/* Función para pasar al hilo que monitoriza e imprime las muertes de
 los filósofos. */
void	*ft_monitoring(void *arg)
{
	t_philo	**philos;
	int		dead;
	int		i;
	long	current_t;

	philos = (t_philo **)arg;
	i = 0;
	while (1)
	{
		if (i >= philos[0]->n_philos)
			i = 0;
		pthread_mutex_lock(philos[0]->dead_mtx);
		if (*(philos[0]->dead))
		{
			dead = *(philos[0]->dead) - 1;
			if (dead >= 0)
				ft_print_dead(philos[dead]);
			pthread_mutex_unlock(philos[0]->dead_mtx);
			break ;
		}
		pthread_mutex_unlock(philos[0]->dead_mtx);
		current_t = ft_get_time();
		pthread_mutex_lock(philos[0]->last_meal_mtx);
		if (current_t - philos[i]->last_meal_ms > philos[i]->t_to_die)
		{
			pthread_mutex_unlock(philos[0]->last_meal_mtx);
			ft_print_dead(philos[i]);
			pthread_mutex_lock(philos[0]->dead_mtx);
			*(philos[i]->dead) = -1;
			pthread_mutex_unlock(philos[0]->dead_mtx);
		}
		else
		pthread_mutex_unlock(philos[0]->last_meal_mtx);
		usleep(100);
		i++;
	}
	return (NULL);
}

void    *ft_calloc(const size_t nmemb, const size_t size)
{
	void	*result;
	size_t  bytes;
	size_t	i;

	if (nmemb == 0 || size == 0)
	{
		result = malloc(0);
		return (result);
	}
	if (nmemb > SIZE_MAX / size)
		return (NULL);
	bytes = nmemb * size;
	result = malloc(bytes);
	if (!result)
		return (write(2, "Error: malloc\n", 14), NULL);
	i = 0;
	while (i < (bytes))
	{
		((char *)result)[i] = '\0';
		i++;
	}
	return (result);
}

int	ft_strlen(const char *s)
{
	int	i;

	i = 0;
	if (!s)
		return (0);
	while (s[i])
		i++;
	return (i);
}

