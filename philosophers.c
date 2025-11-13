#include "philosophers.h"

static t_philo	*ft_check_dead(t_philo *philo_d, int dead, pthread_t	*philo)
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

static int	ft_collect_philos(t_philo *philo_d, pthread_t **threads)
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
	printf("%ld ms %d is dead\n", t2 - t, philo_d->philo);
	pthread_mutex_lock(philo_d->dead_m);
	*(philo_d->dead) = 1;
	pthread_mutex_unlock(philo_d->dead_m);
	return (NULL);
}

static int ft_exec_more_actions(t_philo *philo_d, int fork1, int fork2, int i)
{
	if (philo_d->n_to_eat && i == philo_d->n_to_eat - 1)
	{
		ft_unlock(philo_d, fork1, fork2);
		return (ft_set_dead_m(philo_d, 1), 1);
	}
	ft_unlock(philo_d, fork1, fork2);
	if (ft_time_printer(philo_d, SLEEP) < 0)
	{
		ft_unlock(philo_d, fork1, fork2);
		return (ft_set_dead_m(philo_d, 1), 1);
	}
	ft_usleep(philo_d->t_to_sleep, philo_d);
	if (ft_time_printer(philo_d, THINK) < 0)
	{
		ft_unlock(philo_d, fork1, fork2);
		return (ft_set_dead_m(philo_d, 1), 1);
	}
}

static void	*ft_execute_philo(t_philo *philo_d, int fork1, int fork2)
{
	int	i;

	i = 0;
	while (1)
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

static void	*ft_philo(void *arg)
{
	t_philo	*philo_d;
	int		prev_philo;
	int		fork1;
	int		fork2;

	philo_d = (t_philo *)arg;
	if (philo_d->philo == 1)
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
	return (ft_set_dead_m(philo_d, 1), ft_unlock(philo_d, fork1, fork2), NULL);
}
 
static pthread_mutex_t		*ft_init_forks(int n)
{
	pthread_mutex_t *forks;

	forks = (pthread_mutex_t *)ft_calloc(n, sizeof(pthread_mutex_t));
	if (!forks)
		return (NULL);
	while (n)
	{
		pthread_mutex_init(&(forks[n - 1]), NULL);
		n--;
	}
	return (forks);
}

static void	*ft_init_philo(t_arg *data, t_philo **start)
{
	int	i;
	t_philo *node;

	i = 0;
	node = *start;
	while (i < data->n_philos)
	{
		node->n_philos = data->n_philos;
		node->n_to_eat = data->n_to_eat;
		node->t_to_die = data->t_to_die;
		node->t_to_eat = data->t_to_eat;
		node->t_to_sleep = data->t_to_sleep;
		node->philo = i + 1;
		i++;
		if (i == data->n_philos)
			break ;
		node->next = (t_philo *)ft_calloc(1, sizeof(t_philo));
		if (!node->next)
			return (ft_free_list(start), NULL);
		(node->next)->dead_m = node->dead_m;
		node = node->next;
	}
	if (node)
		node->next = *start;
	return (NULL);
}

static int ft_set_philo(pthread_mutex_t *forks, int *dead, t_philo **start)
{
	t_philo			*node;
	pthread_mutex_t	*printer;
	int				n_philos;
	int				i;

	printer = (pthread_mutex_t *)ft_calloc(1, sizeof(pthread_mutex_t));
	if (!printer)
	{
		ft_free_list(start);
		return (0);
	}
	pthread_mutex_init(printer, NULL);
	i = 0;
	node = *start;
	n_philos = node->n_philos;
	while (i < n_philos)
	{
		node->dead = dead;
		node->printer = printer;
		node->forks = forks;
		node = node->next;
		i++;
	}
	return (1);
}

static t_philo	*ft_init_list(t_arg *data, pthread_mutex_t *forks, int *dead)
{
	t_philo *start;

	start = (t_philo *)ft_calloc(1, sizeof(t_philo));
	if (!start || !forks)
		return (free(forks), ft_free_list(&start), NULL);
	start->dead_m = (pthread_mutex_t *)ft_calloc(1, sizeof(pthread_mutex_t));
	if (!start->dead_m)
		return (free(forks), ft_free_list(&start), NULL);
	pthread_mutex_init(start->dead_m, NULL);
	ft_init_philo(data, &start);
	if (!ft_set_philo(forks, dead, &start))
		return (NULL);
	return (start);
}

static int	ft_create_philos(t_arg *data, int *dead)
{
	pthread_t		*philo;
	t_philo			*philo_d;
	t_philo 		*head;
	int				i;

	philo_d = ft_init_list(data, ft_init_forks(data->n_philos), dead);
	philo = (pthread_t *)ft_calloc(data->n_philos, sizeof(pthread_t));
	if (!philo_d || !philo)
		return (free(philo), ft_free_list(&philo_d), 1);
	i = 0;
	if (ft_set_timer(philo_d))
		return (1);
	head = philo_d;
	while (i < data->n_philos)
	{
		pthread_create(&(philo[i]), NULL, ft_philo, (void *)philo_d);
		philo_d = philo_d->next;
		i++;
	}
	i = 0;
	ft_collect_philos(head, &philo);
	ft_free_list(&head);
	free(philo);
	return (0);
}

int	main(int argc, char **argv)
{
	t_arg	*data;
	int	dead;

	if (argc < 5 || argc > 6)
		return (write(2, ERR_MSG, 128), 1);
	data = (t_arg *)ft_calloc(1, sizeof(t_arg));
	if (!data)
		return (write(2, "Error: malloc\n", 15), 1);
	if (!ft_parse(data, argv))
		return (free(data), 1);
	dead = 0;
	ft_create_philos(data, &dead);
	free(data);
}
