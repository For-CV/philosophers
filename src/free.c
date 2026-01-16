/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael-m <rafael-m@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 18:29:12 by rafael-m          #+#    #+#             */
/*   Updated: 2025/12/06 23:18:19 by rafael-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// Recoge los filósofos una vez han terminado.
void	collect_philos(const pthread_t *threads, const t_philo *philos)
{
	int		dead;
	int		n_philos;

	dead = 0;
	n_philos = philos->n_phil;
	while (dead < n_philos)
	{
		ft_pthread_join(threads[dead]);
		dead++;
	}
}

// Libera y destruye los mutexes, libera y destruye n_philos forks.
void	free_mtxs(t_mtxs *mtxs, int n_philos)
{
	int	i;

	if (!mtxs)
		return ;
	i = 0;
	while (i < n_philos)
	{
		if (mtxs->forks[i])
			ft_mutex_destroy(mtxs->forks[i]);
		free(mtxs->forks[i]);
		i++;
	}
	free(mtxs->forks);
	if (mtxs->dead_m)
		ft_mutex_destroy(mtxs->dead_m);
	free(mtxs->dead_m);
	if (mtxs->last_meal_mtx)
		ft_mutex_destroy(mtxs->last_meal_mtx);
	free(mtxs->last_meal_mtx);
	if (mtxs->printer)
		ft_mutex_destroy(mtxs->printer);
	free(mtxs->printer);
	if (mtxs->finished_mtx)
		ft_mutex_destroy(mtxs->finished_mtx);
	free(mtxs->finished_mtx);
}

// Libera las estructuras de cada filósofo y el array, incluidos
// los mutexes. El flag initiated es para saber si los mutexes
// printer y dead_m están iniciados con pthread_mute_init.
void	free_philos(t_philo *philo)
{
	if (!philo)
		return ;
	if (philo->threads)
		free(philo->threads);
	free(philo);
}
