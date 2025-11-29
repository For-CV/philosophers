#include "philo.h"

int	main(int argc, char **argv)
{
	t_table table;
	t_mtxs	*mtxs;
	t_philo **philos;
	int		dead;

	if (argc < 5 || argc > 6)
		return (write(2, ERR_MSG, 128), 1);
	if (!ft_parse(&table, argv))
		return (1);
	if (table.n_philos == 1)
		return (write(1, "0 ms 1 died\n", 12), 0);
	mtxs = ft_init_mtxs(table.n_philos);
	if (!mtxs)
		return (1);
	dead = 0;
	philos = ft_create_philos(&table, mtxs);
	if (!philos || !ft_init_philos(philos, &table, mtxs, &dead))
		return (ft_free_mtxs(mtxs, table.n_philos), ft_free_philos(philos), 1);
	if (ft_start_sim(philos))
		return (ft_free_mtxs(mtxs, philos[0]->n_philos), ft_free_philos(philos), 1);
	return (ft_free_mtxs(mtxs, philos[0]->n_philos), ft_free_philos(philos), 0);
}
