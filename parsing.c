#include "philosophers.h"

int	ft_parse(t_data *data, char **argv)
{
	int	t;

	data->n_to_eat = 0;
	data->n_philo = ft_atoi(argv[1]);
	if (data->n_philo < 0)
		return (write(2, "n_philo must be an int\n", 23), 0);
	t = ft_atoi(argv[2]);
	if (t < 0)
		return (write(2, "t_to_die must be an int\n", 24), 0);
	data->t_to_die = (__useconds_t)t;
	t = ft_atoi(argv[3]);
	if (data->n_philo < 0)
		return (write(2, "t_to_eat must be an int\n", 24), 0);
	data->t_to_eat = (__useconds_t)t;
	t = ft_atoi(argv[4]);
	if (t < 0)
		return (write(2, "t_to_sleep must be an int\n", 26), 0);
	data->t_to_sleep = (__useconds_t)t;
	if (argv[5])
	{
		data->n_to_eat = ft_atoi(argv[5]);
		if (data->n_to_eat < 0)
			return (write(2, "n_to_eat must be an int\n", 24), 0);
	}
	return (1);
}