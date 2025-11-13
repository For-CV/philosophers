#include "philosophers.h"

t_philo	*ft_check_dead(t_philo *philo_d, int dead, pthread_t *philo)
{
	pthread_mutex_unlock(philo_d->dead_m);
	if (dead)
	{
		dead = 0;
		pthread_join(philo[philo_d->philo - 1], NULL);
	}
	philo_d = philo_d->next;
	pthread_mutex_lock(philo_d->dead_m);
	if (!*(philo_d->dead))
	{
		pthread_mutex_unlock(philo_d->dead_m);
		usleep(100);
	}
	else
		pthread_mutex_unlock(philo_d->dead_m);
	return (philo_d);
}

int	ft_collect_philos(t_philo *philo_d, pthread_t **threads)
{
	pthread_t	*philo;
	int			n_dead;
	int			n_philos;
	int			dead;;

	if (!philo_d || !threads || !*threads)
		return (1);
	philo = *threads;
	n_philos = philo_d->n_philos;
	n_dead = 0;
	dead = 0;
	while (n_dead < n_philos)
	{
		pthread_mutex_lock(philo_d->dead_m);
		if (*(philo_d->dead))
		{
			n_dead++;
			dead = 1;
		}
		philo_d = ft_check_dead(philo_d, dead, philo);
	}
	printf(COLL_MSG);
	return (0);
}