#include "philo.h"

// Sets the same starting time and last_meal_time for all
// philosophers.
// @return 0 if succesful, 1 if gettimeofday fails.
int	ft_set_time(t_philo **philos)
{
	int	i;
	int	n_philos;

	n_philos = philos[0]->n_philos;
	i = 0;
	while (i < n_philos)
	{
		philos[i]->start_ms = ft_get_time();
		if (philos[i]->start_ms == -1)
			return (1);
		philos[i]->last_meal_ms = philos[i]->start_ms;
		i++;
	}
	return (0);
}

/* Gets the current time since Epoch in miliseconds. If gettimeofday fails,
 writes the error message and returns -1 */
long	ft_get_time(void)
{
	struct timeval	tv;
	long			time;

	if (gettimeofday(&tv, NULL))
		return (write(2, "Error: gettimeofday\n", 19), -1);
	time = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	return (time);
}

int	ft_usleep(const long ms, const t_philo *philo)
{
	long	start;

	start = ft_get_time();
	while (1)
	{
		pthread_mutex_lock(philo->dead_m);
		if (*(philo->dead) && *(philo->dead) != -2)
		{
			pthread_mutex_unlock(philo->dead_m);
			return (1);
		}
		        pthread_mutex_unlock(philo->dead_m);
		        if (ft_get_time() - philo->last_meal_ms >= philo->t_to_die)
		            return (2);
		        if (ft_get_time() - start >= ms)			return (0);
		usleep(50);
	}
	return (0);
}