/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael-m <rafael-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 19:08:53 by rafael-m          #+#    #+#             */
/*   Updated: 2026/02/04 20:45:33 by rafael-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// Sets the same starting time and last_meal_time for all
// philosophers.
// @return 0 if successful, 1 if gettimeofday fails.
int	set_time(t_philo *philos)
{
	int		i;
	int		n_philos;
	long	start_time;

	n_philos = philos->n_phil;
	i = 0;
	start_time = get_time();
	while (i < n_philos)
	{
		philos[i].start_ms = start_time;
		philos[i].last_meal_ms = philos[i].start_ms;
		i++;
	}
	return (0);
}

/* Gets the current time since Epoch in miliseconds. */
long	get_time(void)
{
	struct timeval	tv;
	long			time;

	gettimeofday(&tv, NULL);
	time = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	return (time);
}

int	ft_usleep(const long ms, const t_philo *philo)
{
	long	start;
	int		dead;
	int		error;

	start = get_time();
	while (1)
	{
		error = ft_mutex_lock(philo->dead_m);
		dead = *(philo->dead);
		error += ft_mutex_unlock(philo->dead_m);
		if (error || (dead && dead != -2))
			return (1);
		if (get_time() - start >= ms)
			return (0);
		if (usleep(500))
		{
			write(2, "Error: usleep\n", 14);
			return (1);
		}
	}
}
