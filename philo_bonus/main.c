/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael <rafael@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 23:49:25 by rafael-m          #+#    #+#             */
/*   Updated: 2026/02/14 14:01:52 by rafael           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/* @brief Opens the named semaphores for die, printer and seats, then
initializes them in each philo */
/* @return 0 on success, 1 if any sem_open failed (and liberates all
resources, including de t_philo * array) */
static int	init_sem(t_philo *philos, sem_t *forks)
{
	sem_t	*printer;
	sem_t	*die;
	sem_t	*seats;
	int		i;

	die = ft_sem_open("/die", O_CREAT, 0644, 1);
	seats = ft_sem_open("/seats", O_CREAT, 0644,
			(philos->table->n_philos) / 2);
	printer = ft_sem_open("/printer", O_CREAT, 0644, 1);
	if (die == SEM_FAILED || seats == SEM_FAILED || printer == SEM_FAILED)
		return (free_philos(philos), 1);
	i = 0;
	while (i < philos->table->n_philos)
	{
		philos[i].die = die;
		philos[i].seats = seats;
		philos[i].printer = printer;
		philos[i].forks = forks;
		i++;
	}
	return (0);
}

static void	make_sem_name(char *buf, int id)
{
	int	i;
	int	tmp;
	int	len;

	init_name(buf);
	len = 0;
	tmp = id;
	if (tmp == 0)
		len = 1;
	while (tmp > 0)
	{
		tmp /= 10;
		len++;
	}
	tmp = id;
	buf[6 + len] = '\0';
	i = 0;
	while (i < len)
	{
		buf[6 + len - 1 - i] = (tmp % 10) + '0';
		tmp /= 10;
		i++;
	}
}

/* Creates an array of [number of philosophers] t_philo *philo and initializes
each one of them
 RETURN: The pointer to the array on success, NULL on failure, and
liberates resources  */
static t_philo	*create_philos(t_table *table, sem_t *forks)
{
	t_philo	*philos;
	int		i;
	char	sem_name[30];

	if (!forks)
		return (NULL);
	philos = (t_philo *)ft_calloc(table->n_philos, sizeof(t_philo));
	if (!philos)
	{
		close_forks(forks, 0);
		return (NULL);
	}
	i = 0;
	while (i < table->n_philos)
	{
		philos[i].table = table;
		make_sem_name(sem_name, i + 1);
		philos[i].meal_sem = ft_sem_open(sem_name, O_CREAT, 0644, 1);
		i++;
	}
	if (init_sem(philos, forks))
		return (NULL);
	return (philos);
}

/* @brief Unlinks all named semaphores to ensure a clean start */
static void	unlink_all(int n_philos)
{
	char	sem_name[30];
	int		i;

	sem_unlink("/forks");
	sem_unlink("/die");
	sem_unlink("/seats");
	sem_unlink("/printer");
	i = 0;
	while (i < n_philos)
	{
		make_sem_name(sem_name, i + 1);
		sem_unlink(sem_name);
		i++;
	}
}

int	main(const int argc, char **argv)
{
	t_table	table;
	sem_t	*forks;
	t_philo	*philos;
	int		status;

	if (argc < 5 || argc > 6)
		return (write(2, ERR_MSG, 128), 1);
	if (!parser(&table, argv))
		return (1);
	unlink_all(table.n_philos);
	forks = ft_sem_open("/forks", O_CREAT, 0644, table.n_philos);
	if (forks == SEM_FAILED)
		return (1);
	philos = create_philos(&table, forks);
	if (!philos)
		return (1);
	status = start_sim(philos);
	free_philos(philos);
	return (status);
}
