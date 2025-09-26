#include "philosophers.h"

void	ft_print_list(t_philo *l_philo)
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
		printf("n_dead = %d\n", l_philo->n_dead);
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

// int	ft_collect_dead(t_data *data, pthread_t **threads)
// {
// 	pthread_t	*philo;

// 	philo = *threads;
// 	while (1)
// 	{
// 		if (data->dead)
// 		{
// 			pthread_join(philo[data->dead], NULL);
// 			data->n_dead++;
// 			if (data->n_dead == data->n_philo)
// 				return (pthread_mutex_unlock(&(data->global)), 1);
// 		}
// 	}
// 	return (0);
// }

void	*ft_philo(void *arg)
{
	t_data	*data;
	int	philo;
	int	prev_philo;

	// philo = data->philo;
	// if (philo > 0)
	// 	prev_philo = philo - 1;
	// else
	// 	prev_philo = data->n_philo;
	// data = (t_data *)arg;
	// while (1)
	// {
	// 	pthread_mutex_lock(&(data->forks[philo - 1]));
	// 	pthread_mutex_lock(&(data->forks[philo]));

	// }
}

t_philo	*ft_init_list(t_data *data)
{
	t_philo *start;
	t_philo *node;
	pthread_mutex_t *forks;
	int	i;

	forks = (pthread_mutex_t *)ft_calloc(data->n_philos, sizeof(pthread_mutex_t));
	if (!forks)
		return (NULL);
	i = 0;
	start = (t_philo *)ft_calloc(1, sizeof(t_philo));
	if (!start)
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
		node->dead = 0;
		node->n_dead = 0;
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

int	ft_create_philos(t_data *data)
{
	pthread_t	*philo;
	t_philo		*l_philo;
	int			i;

	l_philo = ft_init_list(data);
	philo = (pthread_t *)ft_calloc(data->n_philos, sizeof(pthread_t));
	if (!l_philo || !philo)
		return(1);
	i = 0;
	ft_print_list(l_philo);
	ft_free_list(&l_philo);
	free(philo);
	return (1);
	// while (i < data->n_philos)
	// {
	// 	// pthread_mutex_init(&(l_philo->forks[i]), NULL);
	// 	i++;
	// }
	// i = 0;
	// while (i < data->n_philos)
	// {
	// 	pthread_create(&(philo[i]), NULL, ft_philo, (void *)l_philo);
	// }
	// if (ft_collect_dead(data, &philo))
	// 	return (write(2, DEAD_MSG, 26), 1);
	// return (0);
}

int	main(int argc, char **argv)
{
	t_data	*data;

	if (argc < 5 || argc > 6)
		return (write(2, ERR_MSG, 128), 1);
	data = (t_data *)ft_calloc(1, sizeof(t_data));
	if (!data)
		return (write(2, "malloc failure\n", 15), 1);
	if (!ft_parse(data, argv))
		return (free(data), 1);
	ft_create_philos(data);
	printf("n_philo = %d\n", data->n_philos);
	printf("n_to_eat = %d\n", data->n_to_eat);
	printf("t_to_die = %u\n", data->t_to_die);
	printf("t_to_eat = %u\n", data->t_to_eat);
	printf("t_to_sleep = %u\n", data->t_to_sleep);
	free(data);
}