#include "philosophers.h"

int	ft_parse(t_data *data, char **argv)
{
	int	t;

	data->n_to_eat = 0;
	data->n_philo = ft_atoi(argv[1]);
	printf("argv[2] = %s\n", argv[2]);
	t = ft_atoi(argv[2]);
	printf("t = %d\n", t);
	data->t_to_die = (__useconds_t)t;
	t = ft_atoi(argv[3]);
	data->t_to_eat = (__useconds_t)t;
	t = ft_atoi(argv[4]);
	data->t_to_sleep = (__useconds_t)t;
	if (argv[5])
		data->n_philo = ft_atoi(argv[5]);
	return (1);
}