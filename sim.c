#include "philosophers.h"

static void	ft_unlock(t_philo *philo_d, int first, int second)
{
	if (philo_d->n_philos > 1)
		pthread_mutex_unlock(&(philo_d->forks[second]));
	pthread_mutex_unlock(&(philo_d->forks[first]));
}

static void *ft_one_philo(t_philo *philo_d)
{
	long	t;
	long	t2;

	t = ft_get_time();
	usleep(philo_d->t_to_die * 1000);
	t2 = ft_get_time();
	printf("%ld ms %d  died\n", t2 - t, philo_d->philo);
	pthread_mutex_lock(philo_d->dead_m);
	*(philo_d->dead) = 1;
	pthread_mutex_unlock(philo_d->dead_m);
	return (NULL);
}

static int ft_exec_more_actions(t_philo *philo_d, int fork1, int fork2, int i)
{
	ft_unlock(philo_d, fork1, fork2);
	if (philo_d->n_to_eat && i == philo_d->n_to_eat - 1)
		return (ft_set_dead_m(philo_d, 1), 1);
	if (ft_time_printer(philo_d, SLEEP) < 0)
		return (ft_set_dead_m(philo_d, 1), 1);
	ft_usleep(philo_d->t_to_sleep, philo_d);
	if (ft_time_printer(philo_d, THINK) < 0)
		return (ft_set_dead_m(philo_d, 1), 1);
	return (0);
}

static void	*ft_execute_philo(t_philo *philo_d, int fork1, int fork2)
{
	int	i;
	int	n_to_eat;

	i = 0;
	n_to_eat = philo_d->n_to_eat;
	while (!n_to_eat || i < n_to_eat)
	{
		pthread_mutex_lock(&(philo_d->forks[fork1]));
		if (philo_d->n_philos > 1)
			pthread_mutex_lock(&(philo_d->forks[fork2]));
		if (ft_time_printer(philo_d, FORK) < 0)
		{
			ft_unlock(philo_d, fork1, fork2);
			return (ft_set_dead_m(philo_d, 1), NULL);
		}
		if (ft_time_printer(philo_d, EAT) < 0)
		{
			ft_unlock(philo_d, fork1, fork2);
			return (ft_set_dead_m(philo_d, 1), NULL);
		}
		if (ft_exec_more_actions(philo_d, fork1, fork2, i))
			return (NULL);
		i++;
	}
}

void	*ft_philo(void *arg)
{
	t_philo	*philo_d;
	int		prev_philo;
	int		fork1;
	int		fork2;

	philo_d = (t_philo *)arg;
	if (philo_d->n_philos == 1)
		return (ft_one_philo(philo_d));
	if (philo_d->philo - 1 > 0)
		prev_philo = philo_d->philo - 2;
	else
		prev_philo = philo_d->n_philos - 1;
	if ((philo_d->philo % 2) == 0)
	{
		fork1 = philo_d->philo - 1;
		fork2 = prev_philo;
	}
	else
	{
		fork1 = prev_philo;
		fork2 = philo_d->philo - 1;
	}
	ft_execute_philo(philo_d, fork1, fork2);
	return (ft_set_dead_m(philo_d, 1), NULL);
}
