# include "philosophers.h"

int	ft_set_timer(t_philo *node)
{
	int				i;
	struct timeval	*tv;

	tv = (struct timeval *)ft_calloc(1, sizeof(struct timeval));
	if (!tv)
		return (1);
	i = 0;
	if (gettimeofday(tv, NULL))
		return (free(tv), -1);
	while (node)
	{
		node->start_t = (tv->tv_sec * 1000) + (tv->tv_usec  / 1000);
		node->last_meal_t = node->start_t;
		printf("start_t = %ld\n", node->start_t);
		node = node->next;
	}
	return (free(tv), 0);
}

long	ft_time_printer(t_philo *philo_d, int act)
{
	struct timeval	*tv;
	suseconds_t		current_t;
	suseconds_t		act_t;


	tv = (struct timeval *)ft_calloc(1, sizeof(struct timeval));
	if (!tv)
		return (-1);
	if (gettimeofday(tv, NULL))
		return (free(tv), -1);
	current_t = (((tv->tv_sec * 1000) + (tv->tv_usec / 1000)) - philo_d->start_t);
	if (act == EAT)
		philo_d->last_meal_t = current_t;
	act_t = current_t - philo_d->last_t;
	philo_d->last_t = current_t;
	pthread_mutex_lock(philo_d->printer);
	printf("%ld ms %d", current_t, philo_d->philo);
	if (act == FORK)
		printf(" has taken a fork\n");
	else if (act == EAT && (act_t <= philo_d->t_to_eat && (philo_d->last_meal_t - current_t) <= philo_d->t_to_die))
		printf(" is eating\n");
	else if (act == SLEEP && act_t <= philo_d->t_to_sleep)
		printf(" is sleeping\n");
	else if (act == THINK && act_t <= philo_d->t_to_die)
		printf(" is thinking\n");
	else
	{
		printf(" died\n");
		return (pthread_mutex_unlock(philo_d->printer), free(tv), -1);
	}
	pthread_mutex_unlock(philo_d->printer);
	return (free(tv), act_t);
}