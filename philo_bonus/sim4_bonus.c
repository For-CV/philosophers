/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim4_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael-m <rafael-m@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 10:26:19 by rafael-m          #+#    #+#             */
/*   Updated: 2026/01/16 10:27:24 by rafael-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	*ft_monitoring(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (1)
	{
		ft_sem_wait(philo->meal_sem);
		if (!philo->sim_active)
		{
			ft_sem_post(philo->meal_sem);
			break ;
		}
		if (get_time() - philo->last_meal_ms > philo->table->t_to_die)
		{
			ft_sem_post(philo->meal_sem);
			sem_wait(philo->die);
			sem_wait(philo->printer);
			printf("%ld ms %d died\n", get_time() - philo->start_ms,
				philo->philo_id);
			exit(1);
		}
		ft_sem_post(philo->meal_sem);
		if (usleep(1000))
			break ;
	}
	return (NULL);
}

/* Inicializa start_time y last_meal_t para todos los filósofos.
@return 1 en caso de éxito, 0 en caso de error. */
int	set_time(t_philo *philos)
{
	long	current_t;
	int		n_philos;
	int		i;

	n_philos = philos[0].table->n_philos;
	i = 0;
	current_t = get_time();
	if (current_t < 0)
		return (0);
	while (i < n_philos)
	{
		philos[i].start_ms = current_t;
		philos[i].last_meal_ms = current_t;
		i++;
	}
	return (1);
}

bool	philo_actions(t_philo *philos, t_philo *philo, int *i, int *ret)
{
	if (*i == INT_MAX)
		*i = 0;
	*ret = take_forks(philo);
	if (!*ret)
		*ret = eating(philo);
	(*i)++;
	if (!*ret && philo->table->n_to_eat > 0 && *i >= philo->table->n_to_eat)
		return (true);
	if (!*ret)
		*ret = sleeping(philo, philos);
	if (!*ret)
		*ret = thinking(philo);
	return (false);
}
