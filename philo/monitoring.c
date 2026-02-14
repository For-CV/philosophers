/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael-m <rafael-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 22:17:22 by rafael-m          #+#    #+#             */
/*   Updated: 2026/02/04 20:56:56 by rafael-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* Imprime la muerte de un filósofo. */
static void	print_dead(const t_philo *philo)
{
	long	current_t;

	ft_mutex_lock(philo->printer);
	current_t = get_time();
	if (current_t >= 0)
		putlng_fd(current_t - philo->start_ms, 1);
	else
		putlng_fd(current_t, 1);
	write(1, " ms ", 4);
	putlng_fd((long)philo->philo_id, 1);
	write(1, " died\n", 6);
	ft_mutex_unlock(philo->printer);
}

/* Imprime las muertes y pone el actualiza el flag int *dead */
static int	monitor_dead(const t_philo *philos, int i)
{
	long	current_t;

	ft_mutex_lock(philos[0].finished_mtx);
	if (philos[i].finished)
		return (ft_mutex_unlock(philos[0].finished_mtx), 0);
	ft_mutex_unlock(philos[0].finished_mtx);
	ft_mutex_lock(philos[0].last_meal_mtx);
	current_t = get_time();
	if (current_t - philos[i].last_meal_ms > philos[i].t_to_die)
	{
		ft_mutex_unlock(philos[0].last_meal_mtx);
		ft_mutex_lock(philos[0].dead_m);
		if (!*(philos[0].dead))
		{
			*(philos[i].dead) = i + 1;
			ft_mutex_unlock(philos[0].dead_m);
			print_dead(&philos[i]);
		}
		else
			ft_mutex_unlock(philos[0].dead_m);
	}
	else
		ft_mutex_unlock(philos[0].last_meal_mtx);
	return (0);
}

/* Función para pasar al hilo que monitoriza e imprime las muertes de
 los filósofos. */
void	*ft_monitoring(void *arg)
{
	t_philo	*philos;
	int		dead;
	int		i;

	philos = (t_philo *)arg;
	i = 0;
	while (1)
	{
		if (i >= philos[0].n_phil)
		{
			usleep(100);
			i = 0;
		}
		ft_mutex_lock(philos[0].dead_m);
		dead = *(philos[0].dead);
		if (dead)
		{
			pthread_mutex_unlock(philos[0].dead_m);
			break ;
		}
		ft_mutex_unlock(philos[0].dead_m);
		monitor_dead(philos, i);
		i++;
	}
	return (NULL);
}
