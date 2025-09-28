# include "philosophers.h"

long	ft_get_time(void)
{
	struct timeval	*tv;
	long			time;

	tv = (struct timeval *)ft_calloc(1, sizeof(struct timeval));
	if (!tv)
		return (-1);
	if (gettimeofday(tv, NULL))
		return (free(tv), -1);
	time = (tv->tv_sec * 1000) + (tv->tv_usec  / 1000);
	free(tv);
	return (time);
}

void	ft_usleep(long ms, t_philo *philo_d)
{
	long	start;

	start = ft_get_time();
	while (!philo_d->dead)
	{
		if (ft_get_time() - start >= ms)
			break ;
		if (ms > 100)
			usleep(100);
		else
		{
			usleep(ms);
			return ;
		}
	}
}

int	ft_set_timer(t_philo *node)
{
	int		i;
	long	time;

	time = ft_get_time();
	if (time < 0)
		return (-1);
	i = 0;
	while (node && i < node->n_philos)
	{
		node->start_t = time;
		node->last_meal_t = node->start_t;
		node = node->next;
		i++;
	}
	return (0);
}

long	ft_time_printer(t_philo *philo_d, int act)
{
	suseconds_t		current_t;
	suseconds_t		act_t;

	current_t = ft_get_time();
	if (current_t < 0)
		return (-1);
	act_t = current_t  - philo_d->start_t - philo_d->last_t;
	philo_d->last_t = current_t - philo_d->start_t;
	if ((current_t - philo_d->last_meal_t) >= philo_d->t_to_die)
		act = -1;
	pthread_mutex_lock(philo_d->printer);
	if (!philo_d->n_to_eat && philo_d->dead)
		return (pthread_mutex_unlock(philo_d->printer), -1);
	printf("%ld ms %d", current_t - philo_d->start_t, philo_d->philo);
	if (act == FORK)
		printf(" has taken a fork\n");
	else if (act == EAT && act_t <= philo_d->t_to_eat)
	{
		philo_d->last_meal_t = current_t;
		printf(" is eating\n");
	}
	else if (act == SLEEP && act_t <= philo_d->t_to_sleep)
		printf(" is sleeping\n");
	else if (act == THINK && act_t <= philo_d->t_to_die)
		printf(" is thinking\n");
	else
	{
		printf(" died\n");
		if (philo_d->n_to_eat)
			pthread_mutex_unlock(philo_d->printer);
		return (-1);
	}
	pthread_mutex_unlock(philo_d->printer);
	return (act_t);
}