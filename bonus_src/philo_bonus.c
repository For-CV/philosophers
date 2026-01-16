/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael-m <rafael-m@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 23:49:25 by rafael-m          #+#    #+#             */
/*   Updated: 2026/01/16 01:34:01 by rafael-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/* @brief Opens the named semaphores for die, printer and seats, then
initializes them in each philo */
/* @return 0 on success, 1 if any sem_open failed (and liberates all
resources, including de t_philo * array) */
static int	init_sem(t_philo **philos, sem_t *forks)
{
	sem_t	*printer;
	sem_t	*die;
	sem_t	*seats;
	int		i;

	die = ft_sem_open("/die", O_CREAT | O_EXCL, 0644, 1);
	seats = ft_sem_open("/seats", O_CREAT, 0644,
			((*philos)->table->n_philos + 1) / 2);
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
		return (free_philos(philos), 1);
	return (0);
}

/* Creates an array of [number of philosophers] t_philo *philo and initializes
each one of them */
/* @return The pointer to the array on success, NULL on failure, and 
liberates resources  */
static t_philo	**create_philos(t_table *table, sem_t *forks)
{
	t_philo	**philos;
	int		i;

	if (!forks)
		return (NULL);
	philos = (t_philo **)ft_calloc(table->n_philos, sizeof(t_philo *));
	if (!philos)
	{
		close_forks(forks, 0);
		return (NULL);
	}
	i = 0;
	while (i < table->n_philos)
	{
		philos[i] = (t_philo *)ft_calloc(1, sizeof(t_philo));
		if (!philos[i])
			return (free_when_creating(philos, forks), NULL);
		philos[i]->table = table;
		pthread_mutex_init(&philos[i]->meal_mtx, NULL);
		i++;
	}
	if (init_sem(philos, forks))
		return (NULL);
	return (philos);
}

int	main(const int argc, char **argv)
{
	t_table	table;
	sem_t	*forks;
	t_philo	**philos;
	int		status;

	if (argc < 5 || argc > 6)
		return (write(2, ERR_MSG, 128), 1);
	if (!parser(&table, argv))
		return (1);
	forks = ft_sem_open("/forks", O_CREAT | O_EXCL, 0644, table.n_philos);
	if ((forks) == SEM_FAILED)
		return (close_forks(forks, 0), 1);
	philos = create_philos(&table, forks);
	if (!philos)
		return (1);
	status = start_sim(philos);
	status += ft_sem_close((*philos)->die);
	free_philos(philos);
	return (status);
}
