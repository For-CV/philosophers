#include "philosophers.h"

int	main(int argc, char **argv)
{
	t_data	*data;

	if (argc < 5 || argc > 6)
		return (write(2, ERR_MSG, 128), 1);
	data = (t_data *)ft_calloc(1, sizeof(t_data));
	if (!data)
		return (write(2, "malloc failure\n", 15), 1);
	ft_parse(data, argv);
	printf("n_philo = %d\n", data->n_philo);
	printf("n_to_eat = %d\n", data->n_to_eat);
	printf("t_to_die = %u\n", data->t_to_die);
	printf("t_to_eat = %u\n", data->t_to_eat);
	printf("t_to_sleep = %u\n", data->t_to_sleep);
	free(data);
}