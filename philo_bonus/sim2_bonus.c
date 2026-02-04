/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim2_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael-m <rafael-m@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 23:49:25 by rafael-m          #+#    #+#             */
/*   Updated: 2025/12/06 23:49:25 by rafael-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/* @brief No-op check since simulation relies on parent kill and printer lock */
/* @return 0 */
int	check_dead(const t_philo *philo)
{
	(void)philo;
	return (0);
}

/* @brief Semwaits for seats, two forks and printer, writing
corresponding error messages */
/* @return 0 if everything went ok, 1 if any sem_wait error */
static int	ft_sems_wait(const t_philo *philo)
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
	printf("%ld ms %d has taken a fork\n", t - philo->start_ms,
		philo->philo_id);
	ft_sem_post(philo->printer);
	return (0);
}

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
/* @return 0 if everything went ok, 1 if any semaphore operation failed
or checked that a philosopher died */
int	take_forks(t_philo *philo)
{
	long	start_t;
	int		dead;

	start_t = get_time();
	if (start_t < 0)
		return (1);
	if (ft_sems_wait(philo))
		return (1);
	ft_sem_wait(philo->meal_sem);
	if (start_t - philo->last_meal_ms <= philo->table->t_to_die)
	{
		ft_sem_post(philo->meal_sem);
		dead = 0;
	}
	else
	{
		ft_sem_post(philo->meal_sem);
		sem_wait(philo->die);
		sem_wait(philo->printer);
		printf("%ld ms %d died\n", start_t - philo->start_ms, philo->philo_id);
		exit(1);
	}
	return (dead);
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
