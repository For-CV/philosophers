/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael-m <rafael-m@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 18:29:12 by rafael-m          #+#    #+#             */
/*   Updated: 2025/12/05 18:29:12 by rafael-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/* @brief Releases resources when philosopher initialization fails.
Frees the shared table, closes/unlinks the forks semaphore, and destroys
every `t_philo` node that was already allocated. */
void	free_when_creating(t_philo *philos, sem_t *forks)
{
	if (!philos)
		return ;
	close_forks(forks, 0);
	free_philos(philos);
	return ;
}

/* @brief Closes the forks semaphore and unlinks it on the parent process. */
void	close_forks(sem_t *forks, int child)
{
	if (forks)
	{
		ft_sem_close(forks);
		if (!child)
			ft_sem_unlink("/forks");
	}
	return ;
}

/* @brief Frees all child-process resources.
Closes the printer/seats semaphores, releases the table copy and every
`t_philo`, and closes the shared forks semaphore without unlinking. */
void	free_child(t_philo *philos)
{
	int	i;
	int	n_philos;

	if (!philos)
		return ;
	i = 0;
	n_philos = 0;
	ft_sem_close(philos[i].printer);
	ft_sem_close(philos[i].seats);
	n_philos = philos[0].table->n_philos;
	close_forks(philos->forks, 1);
	while (i < n_philos)
	{
		ft_sem_close(philos[i].meal_sem);
		i++;
	}
	free(philos);
}

/* @brief Closes and unlinks the shared semaphores held by the parent. */
/* Frees `philos[0]->printer` and `philos[0]->seats` handles plus `/forks`. */
static void	close_sems(t_philo *philos)
{
	if (!philos)
		return ;
	if (philos[0].printer != SEM_FAILED)
		ft_sem_close(philos[0].printer);
	if (philos[0].seats != SEM_FAILED)
		ft_sem_close(philos[0].seats);
	if (philos[0].printer)
		ft_sem_unlink("/printer");
	if (philos[0].seats)
		ft_sem_unlink("/seats");
	if (philos[0].die != SEM_FAILED)
		ft_sem_close(philos[0].die);
	if (philos[0].die)
		ft_sem_unlink("/die");
	close_forks(philos->forks, 0);
}

/* @brief Fully frees the philosophers array and shared table
in the parent. Closes and unlinks semaphores via `ft_close_sems`
 and frees every `t_philo` entry and the array wrapper. */
void	free_philos(t_philo *philos)
{
	int	i;
	int	n_philos;

	if (!philos)
		return ;
	i = 0;
	n_philos = 0;
	close_sems(philos);
	if (philos[0].table)
		n_philos = philos[0].table->n_philos;
	while (i < n_philos)
	{
		ft_sem_close(philos[i].meal_sem);
		i++;
	}
	free(philos);
	return ;
}
