#include "philosophers.h"
 
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
