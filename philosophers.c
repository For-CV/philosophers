#include "philosophers.h"

void	*ft_philo(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	while (1)
	{

	}
}

int	ft_init_philo(t_data *data)
{
	pthread_mutex_t	*forks;

	forks = (pthread_mutex_t *)ft_calloc(data->n_philo, sizeof(pthread_mutex_t));
	if (!forks)
		return(1);
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
	ft_init_philo(data);
	printf("n_philo = %d\n", data->n_philo);
	printf("n_to_eat = %d\n", data->n_to_eat);
	printf("t_to_die = %u\n", data->t_to_die);
	printf("t_to_eat = %u\n", data->t_to_eat);
	printf("t_to_sleep = %u\n", data->t_to_sleep);
	free(data);
}