/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim2_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael <rafael@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 23:49:25 by rafael-m          #+#    #+#             */
/*   Updated: 2026/02/14 14:14:04 by rafael           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/* @brief Sem_posts for seats and two forks, writing
corresponding error messages if necessary */
/* @return 0 if everything went ok, 1 if any sem_post error */
int	ft_sems_post(const t_philo *philo)
{
	int	ret;

	ret = ft_sem_post(philo->forks);
	ret += ft_sem_post(philo->forks);
	ret += ft_sem_post(philo->seats);
	return (ret);
}

/* Simulation of taking two forks (with sem_wait) */
/* @return 0 if everything went ok, 1 if any semaphore operation failed */
int	take_forks(t_philo *philo)
{
	long	t;

	if (ft_sem_wait(philo->seats))
		return (1);
	if (ft_sem_wait(philo->forks))
	{
		ft_sem_post(philo->seats);
		return (1);
	}
	t = get_time();
	ft_sem_wait(philo->printer);
	printf("%ld ms %d has taken a fork\n", t - philo->start_ms,
		philo->philo_id);
	ft_sem_post(philo->printer);
	if (ft_sem_wait(philo->forks))
	{
		ft_sem_post(philo->forks);
		ft_sem_post(philo->seats);
		return (1);
	}
	ft_sem_wait(philo->printer);
	t = get_time();
	printf("%ld ms %d has taken a fork\n", t - philo->start_ms,
		philo->philo_id);
	ft_sem_post(philo->printer);
	return (0);
}

/* @brief Simulation of sleeping time_to_sleep milliseconds */
/* @return 0 if everything went ok, 1 if any semaphore operation failed
or checked that a philosopher died  */
int	sleeping(const t_philo *philo, t_philo *philos)
{
	int		dead;
	int		philo_id;
	long	time;

	(void)philos;
	philo_id = philo->philo_id;
	time = get_time();
	if (ft_sem_wait(philo->printer))
		return (1);
	printf("%ld ms %d is sleeping\n", time - philo->start_ms, philo_id);
	dead = ft_sem_post(philo->printer);
	if (ft_usleep(philo->table->t_to_sleep))
		dead = 1;
	return (dead);
}
