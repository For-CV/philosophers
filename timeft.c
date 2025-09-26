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
		return (free(tv), 1);
	while (node)
	{
		node->big_t = tv->tv_sec;
		node->micro_t = tv->tv_usec;
		// printf("node->big_t = %ld, node->micro_t = %ld\n", node->big_t, node->micro_t);
		node = node->next;
	}
	return (free(tv), 0);
}

int	ft_time_printer(suseconds_t micro_t, time_t big_t, t_philo *philo, int act)
{
	struct timeval	*tv;
	long			sec;
	long			t;


	tv = (struct timeval *)ft_calloc(1, sizeof(struct timeval));
	if (!tv)
		return (1);
	if (gettimeofday(tv, NULL))
		return (free(tv), 1);
	sec = (tv->tv_sec - big_t) * 1000;
	t = (sec + (tv->tv_usec - micro_t) / 1000);
	pthread_mutex_lock(philo->printer);
	printf("%ld ms ", t);
	printf("%d", philo->philo);
	if (act == FORK)
		printf(" has taken a fork\n");
	else if (act == EAT && t <= philo->t_to_eat)
		printf(" is eating\n");
	else if (act == SLEEP && t <= philo->t_to_sleep)
		printf(" is sleeping\n");
	else if (act == THINK && t <= philo->t_to_die)
		printf(" is thinking\n");
	else
	{
		printf(" died\n");
		return (pthread_mutex_unlock(philo->printer), free(tv), 1);
	}
	pthread_mutex_unlock(philo->printer);
	return (free(tv), 0);
}