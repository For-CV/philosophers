#include "philosophers.h"

static void	ft_print_list(t_philo *l_philo)
{
	t_philo	*start;
	
	start = l_philo;
	while (1)
	{
		printf("---------- NEW PHILOSOPHER DATA ------------\n");
		printf("n_philos = %d\n", l_philo->n_philos);
		printf("philo = %d\n", l_philo->philo);
		printf("n_to_eat = %d\n", l_philo->n_to_eat);
		printf("dead = %d\n", l_philo->dead);
		printf("t_to_die = %ld\n", l_philo->t_to_die);
		printf("t_to_eat = %ld\n", l_philo->t_to_eat);
		printf("t_to_sleep = %ld\n", l_philo->t_to_sleep);
		printf("forks = %p\n", l_philo->forks);
		printf("philo = %p, next = %p\n", l_philo, l_philo->next);
		printf("\n\n");
		l_philo = l_philo->next;
		if (l_philo == start)
			return ;
	}
}



static int	ft_collect_dead(t_philo *l_philo, pthread_t **threads)
{
	pthread_t	*philo;
	int			n_dead;
	int			n_philos;

	if (!l_philo || !threads || !*threads)
		return (1);
	philo = *threads;
	n_philos = l_philo->n_philos;
	n_dead = 0;
	while (n_dead < n_philos)
	{
		if (l_philo->dead)
		{
			if (!pthread_join(philo[l_philo->philo - 1], NULL))
			{
				n_dead++;
				l_philo->dead = 0;
			}
		}
		l_philo = l_philo->next;
		if (!l_philo->dead)
			usleep(100);
	}
	printf(COLL_MSG);
	return (0);
}

static void	ft_unlock(t_philo *philo_d, int first, int second)
{
	pthread_mutex_unlock(&(philo_d->forks[second]));
	if (philo_d->n_philos > 1)
		pthread_mutex_unlock(&(philo_d->forks[first]));
}

static void	*ft_philo(void *arg)
{
	t_philo	*philo_d;
	int		philo;
	int		prev_philo;
	int		first_fork;
	int		second_fork;
	int		i;
	struct timeval	*tv;
	long	t;

	tv = (struct timeval *)ft_calloc(1, sizeof(struct timeval));
	if (!tv)
		return (NULL);
	philo_d = (t_philo *)arg;
	philo = philo_d->philo - 1;
	if (philo > 0)
		prev_philo = philo - 1;
	else
		prev_philo = philo_d->n_philos - 1;
	if ((philo_d->philo % 2) == 0)
	{
		first_fork = philo;
		second_fork = prev_philo;
	}
	else
	{
		first_fork = prev_philo;
		second_fork = philo;
	}
	i = 0;
	while (i < philo_d->n_to_eat)
	{
		pthread_mutex_lock(&(philo_d->forks[first_fork]));
		if (philo_d->n_philos > 1)
			pthread_mutex_lock(&(philo_d->forks[second_fork]));
		t = ft_time_printer(philo_d->micro_t, philo_d->big_t, philo_d, FORK);
		if (t < 0)
			return (free(tv), philo_d->dead = 1, ft_unlock(philo_d, first_fork, second_fork), NULL);
		t = ft_time_printer(philo_d->micro_t, philo_d->big_t, philo_d, EAT);
		if (t < 0)
			return (free(tv), philo_d->dead = 1, ft_unlock(philo_d, first_fork, second_fork), NULL);
		ft_unlock(philo_d, first_fork, second_fork);
		t = ft_time_printer(philo_d->micro_t, philo_d->big_t, philo_d, SLEEP);
		if (t < 0)
			return (free(tv), philo_d->dead = 1, NULL);
		usleep(philo_d->t_to_sleep);
		t = ft_time_printer(philo_d->micro_t, philo_d->big_t, philo_d, THINK);
		if (t < 0)
			return (free(tv), philo_d->dead = 1, NULL);
		i++;
	}
	return (free(tv), philo_d->dead = 1, NULL);
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

static t_philo	*ft_init_list(t_arg *data, pthread_mutex_t *forks, pthread_mutex_t *printer)
{
	t_philo *start;
	t_philo *node;
	int	i;

	i = 0;
	start = (t_philo *)ft_calloc(1, sizeof(t_philo));
	if (!start || !forks || !printer)
		return (free(forks), NULL);
	node = start;
	while (i < data->n_philos)
	{
		node->n_philos = data->n_philos;
		node->n_to_eat = data->n_to_eat;
		node->t_to_die = data->t_to_die;
		node->t_to_eat = data->t_to_eat;
		node->t_to_sleep = data->t_to_sleep;
		node->philo = i + 1;
		node->forks = forks;
		node->printer = printer;
		i++;
		if (i == data->n_philos)
			break ;
		node->next = (t_philo *)ft_calloc(1, sizeof(t_philo));
		if (!node->next)
			return (ft_free_list(&start), NULL);
		node = node->next;
	}
	if (ft_set_timer(start))
		return (ft_free_list(&start), NULL);
	return (node ->next = start, start);
}

static int	ft_create_philos(t_arg *data)
{
	pthread_t		*philo;
	t_philo			*l_philo;
	pthread_mutex_t	*printer;
	int				i;

	printer = (pthread_mutex_t *)ft_calloc(1, sizeof(pthread_mutex_t));
	l_philo = ft_init_list(data, ft_init_forks(data->n_philos), printer);
	philo = (pthread_t *)ft_calloc(data->n_philos, sizeof(pthread_t));
	if (!l_philo || !philo || !printer)
		return(free(philo), free(printer), ft_free_list(&l_philo), 1);
	pthread_mutex_init(printer, NULL);
	ft_print_list(l_philo);
	i = 0;
	while (i < data->n_philos)
	{
		pthread_create(&(philo[i]), NULL, ft_philo, (void *)l_philo);
		l_philo = l_philo->next;
		i++;
	}
	i = 0;
	ft_collect_dead(l_philo, &philo);
	ft_free_list(&l_philo);
	free(philo);
	free(printer);
	return (0);
}

int	main(int argc, char **argv)
{
	t_arg	*data;

	if (argc < 5 || argc > 6)
		return (write(2, ERR_MSG, 128), 1);
	data = (t_arg *)ft_calloc(1, sizeof(t_arg));
	if (!data)
		return (write(2, "malloc failure\n", 15), 1);
	if (!ft_parse(data, argv))
		return (free(data), 1);
	ft_create_philos(data);
	free(data);
}
