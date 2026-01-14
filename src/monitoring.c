/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael-m <rafael-m@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 22:17:22 by rafael-m          #+#    #+#             */
/*   Updated: 2025/12/06 22:35:32 by rafael-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* Imprime la muerte de un filósofo. */
static void	ft_print_dead(const t_philo *philo)
{
	long	current_t;

	ft_mutex_lock(philo->printer);
	current_t = ft_get_time();
	if (current_t >= 0)
		ft_putlng_fd(current_t - philo->start_ms, 1);
	else
		ft_putlng_fd(current_t, 1);
	write(1, " ms ", 4);
	ft_putlng_fd((long)philo->philo_id, 1);
	write(1, " died\n", 6);
	ft_mutex_unlock(philo->printer);
}

/* Imprime las muertes y pone el actualiza el flag int *dead */
static void	ft_monitor_dead(t_philo *philos, int i)
{
	long	current_t;

	ft_mutex_lock(philos[0].last_meal_mtx);
	current_t = ft_get_time();
	if (current_t - philos[i].last_meal_ms > philos[i].t_to_die)
	{
		ft_mutex_unlock(philos[0].last_meal_mtx);
		ft_mutex_lock(philos[0].dead_m);
		if (!*(philos[0].dead))
		{
			*(philos[i].dead) = i + 1;
			ft_mutex_unlock(philos[0].dead_m);
			ft_print_dead(&philos[i]);
		}
		else
			ft_mutex_unlock(philos[0].dead_m);
	}
	else
		ft_mutex_unlock(philos[0].last_meal_mtx);
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
		if (i >= philos[0].n_philos)
			i = 0;
		ft_mutex_lock(philos[0].dead_m);
		dead = *(philos[0].dead);
		if (dead)
		{
			pthread_mutex_unlock(philos[0].dead_m);
			break ;
		}
		ft_mutex_unlock(philos[0].dead_m);
		ft_monitor_dead(philos, i);
		usleep(100);
		i++;
	}
	return (NULL);
}
