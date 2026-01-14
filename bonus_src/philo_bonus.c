// #include "philo_bonus.h"


/* @brief Opens the named semaphores for die, printer and seats, then
initializes them in each philo */
/* @return 0 on succes, 1 if any sem_open failed (and liberates all 
resources, incluiding de t_philo * array) */
static int	ft_init_sem(t_philo **philos, sem_t *forks)
{
	sem_t	*printer;
	sem_t	*die;
	sem_t	*seats;
	int		i;

	die = ft_sem_open("/die", O_CREAT | O_EXCL, 0644, 1);
	seats = ft_sem_open("/seats", O_CREAT, 0644, (*philos)->table->n_philos / 2);
	printer = ft_sem_open("/printer", O_CREAT | O_EXCL, 0644, 1);
	i = 0;
	while (i < philos[0]->table->n_philos)
	{
		philos[i]->die = die;
		philos[i]->seats = seats;
		philos[i]->printer = printer;
		philos[i]->forks = forks;
		i++;
	}
	if (die == SEM_FAILED || seats == SEM_FAILED || printer == SEM_FAILED)
		return (ft_free_philos(philos), 1);
	return (0);
}

/* Creates an array of [number of philosophers] t_philo *philo and initializes
each one of them */
/* @return The pointer to the array on success, NULL on failure, and 
liberates resources  */
static t_philo	**ft_create_philos(t_table *table, sem_t *forks)
{
	t_philo	**philos;
	int		i;

	if (!table || !forks)
		return (NULL);
	philos = (t_philo **)ft_calloc(table->n_philos, sizeof(t_philo *));
	if (!philos)
	{
		ft_close_forks(forks, 0);
		return (NULL);
	}
	i = 0;
	while (i < table->n_philos)
	{
		philos[i] = (t_philo *)ft_calloc(1, sizeof(t_philo));
		if (!philos[i])
			return (ft_free_when_creating(philos, forks), NULL);
		philos[i]->table = table;
		i++;
	}
	if (ft_init_sem(philos, forks))
		return (NULL);
	return (philos);
}

int	main(int argc, char **argv)
{
	t_table	table;
	sem_t 	*forks;
	t_philo	**philos;
	int		status;

	if (argc < 5 || argc > 6)
		return (write(2, ERR_MSG, 128), 1);
	if (!ft_parse(&table, argv))
		return (1);
	if (table.n_philos == 1)
		return (write(1, "0 ms 1 died\n", 12), 0);
	forks = ft_sem_open("/forks",  O_CREAT | O_EXCL, 0644, table.n_philos);
	if ((forks) == SEM_FAILED)
		return (ft_close_forks(forks, 0), 1);
	philos = ft_create_philos(&table, forks);
	if (!philos)
		return (1);
	status = ft_start_sim(philos);
	status += ft_sem_close((*philos)->die);
	ft_free_philos(philos);
	return (status);
}
