#include "philosophers.h"

static void	ft_print_list(t_philo *philo_d)
{
	t_philo	*start;
	
	start = philo_d;
	while (1)
	{
		printf("---------- NEW PHILOSOPHER DATA ------------\n");
		printf("n_philos = %d\n", philo_d->n_philos);
		printf("philo = %d\n", philo_d->philo);
		printf("n_to_eat = %d\n", philo_d->n_to_eat);
		printf("dead = %d\n", philo_d->dead);
		printf("t_to_die = %ld\n", philo_d->t_to_die);
		printf("t_to_eat = %ld\n", philo_d->t_to_eat);
		printf("t_to_sleep = %ld\n", philo_d->t_to_sleep);
		printf("forks = %p\n", philo_d->forks);
		printf("philo = %p, next = %p\n", philo_d, philo_d->next);
		printf("\n\n");
		philo_d = philo_d->next;
		if (philo_d == start)
			return ;
	}
}

static int	ft_set_dead(t_philo *philo_d)
{
	int	i;

	i = 0;
	while (i < philo_d->n_philos && philo_d)
	{
		philo_d->dead = 1;
		i++;
		philo_d = philo_d->next;
	}
	return (0);
}

static int	ft_collect_philos(t_philo *philo_d, pthread_t **threads)
{
	pthread_t	*philo;
	int			n_dead;
	int			n_philos;

	if (!philo_d || !threads || !*threads)
		return (1);
	philo = *threads;
	n_philos = philo_d->n_philos;
	n_dead = 0;
	while (n_dead < n_philos)
	{
		if (philo_d->dead)
		{
			if (!pthread_join(philo[philo_d->philo - 1], NULL))
			{
				n_dead++;
				if (philo_d->n_to_eat)
					philo_d->dead = 0;
				if (!philo_d->n_to_eat)
				{
					ft_set_dead(philo_d);
					pthread_mutex_unlock(philo_d->printer);
				}
			}
		}
		philo_d = philo_d->next;
		if (!philo_d->dead)
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
	int		prev_philo;
	int		fork1;
	int		fork2;
	int		i;
	long	t;

	philo_d = (t_philo *)arg;
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
	i = 0;
	while (1)
	{
		pthread_mutex_lock(&(philo_d->forks[fork1]));
		if (philo_d->n_philos > 1)
			pthread_mutex_lock(&(philo_d->forks[fork2]));
		t = ft_time_printer(philo_d, FORK);
		if (t < 0)
			return (philo_d->dead = 1, ft_unlock(philo_d, fork1, fork2), NULL);
		t = ft_time_printer(philo_d, EAT);
		if (t < 0)
			return (philo_d->dead = 1, ft_unlock(philo_d, fork1, fork2), NULL);
		if (philo_d->n_to_eat && i == philo_d->n_to_eat - 1)
			return (philo_d->dead = 1, ft_unlock(philo_d, fork1, fork2), NULL);
		ft_unlock(philo_d, fork1, fork2);
		t = ft_time_printer(philo_d, SLEEP);
		if (t < 0)
			return (philo_d->dead = 1, NULL);
		ft_usleep(philo_d->t_to_sleep / 1000, philo_d);
		t = ft_time_printer(philo_d, THINK);
		if (t < 0)
			return (philo_d->dead = 1, NULL);
		i++;
	}
	return (philo_d->dead = 1, NULL);
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
	int	*dead;

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
	return (node ->next = start, start);
}

static int	ft_create_philos(t_arg *data)
{
	pthread_t		*philo;
	t_philo			*philo_d;
	pthread_mutex_t	*printer;
	int				i;

	printer = (pthread_mutex_t *)ft_calloc(1, sizeof(pthread_mutex_t));
	philo_d = ft_init_list(data, ft_init_forks(data->n_philos), printer);
	philo = (pthread_t *)ft_calloc(data->n_philos, sizeof(pthread_t));
	if (!philo_d || !philo || !printer)
		return(free(philo), free(printer), ft_free_list(&philo_d), 1);
	pthread_mutex_init(printer, NULL);
	// ft_print_list(philo_d);
	i = 0;
	if (ft_set_timer(philo_d))
		return (1);
	while (i < data->n_philos)
	{
		pthread_create(&(philo[i]), NULL, ft_philo, (void *)philo_d);
		philo_d = philo_d->next;
		i++;
	}
	i = 0;
	ft_collect_philos(philo_d, &philo);
	ft_free_list(&philo_d);
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
