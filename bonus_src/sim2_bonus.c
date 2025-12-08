/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim2_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael-m <rafael-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 15:55:52 by rafael-m          #+#    #+#             */
/*   Updated: 2025/12/07 18:09:56 by rafael-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/* @brief Checks if any philosopher has died and prints this philosopher's dead
 if true. */
/* @return -1 if another philosopher has died, 1 if this philosopher dies 
while checking dead, else 0 */
int	ft_check_dead(const t_philo *philo)
{
	sem_t	*s;
	long	time;

	errno = 0;
	time = ft_get_time();
	if (time == -1)
		return (-1);
	s = sem_open("/die", 0, 0, 0);
	if (s == SEM_FAILED && errno == ENOENT)
	{
		if (time > LONG_MAX)
			printf("%ld ms %d died\n", time - philo->start_ms, philo->philo_id);
		return (-1);
	}
	if (s != SEM_FAILED)
	{
		if (ft_sem_close(s))
			return (-1);
	}
	return (0);
}

/* @brief Semwaits for seats, two forks and printer, writing
corresponding error messages */
/* @return 0 if everithing went ok, 1 if any sem_wait error */
static int	ft_sems_wait(const t_philo *philo)
{
	if (ft_sem_wait(philo->seats) < 0)
		return (1);
	if (ft_check_dead(philo))
		return (ft_sem_post(philo->seats) + 1);
	if (ft_sem_wait(philo->forks) < 0)
		return (ft_sem_post(philo->seats) + 1);
	if (ft_sem_wait(philo->forks) < 0)
		return (ft_sem_post(philo->forks) + ft_sem_post(philo->seats) + 1);
	if (sem_wait(philo->printer) < 0)
	{
		ft_sem_post(philo->forks);
		ft_sem_post(philo->forks);
		ft_sem_post(philo->seats);
		return (1);
	}
	return (0);
}

/* @brief Sem_posts for seats and two forks, writing
corresponding error messages if necessary */
/* @return 0 if everithing went ok, 1 if any sem_post error */
int	ft_sems_post(const t_philo *philo)
{
	int	ret;

	ret = ft_sem_post(philo->forks);
	ret += ft_sem_post(philo->forks);
	ret += ft_sem_post(philo->seats);
	ret += ft_sem_post(philo->printer);
	return (ret);
}

/* Simulation of taking two forks (with sem_wait) */
/* @return 0 if everything went ok, 1 if any semaphore operation failed
or checked that a philosopher died */
int	ft_takeforks(const t_philo *philo)
{
	int		id;
	long	start_t;
	int		dead;

	id = philo->philo_id;
	start_t = ft_get_time();
	if (start_t < 0)
		return (1);
	if (ft_sems_wait(philo))
		return (1);
	dead = ft_check_dead(philo);
	if (!dead && (start_t - philo->last_meal_ms <= philo->table->t_to_die))
	{
		printf("%ld ms %d has taken fork\n", start_t - philo->start_ms, id);
		dead = ft_sem_post(philo->printer);
	}
	else
	{
		if (dead == 1)
			printf("%ld ms %d died\n", start_t - philo->start_ms, id);
		ft_sem_post(philo->printer);
		if (sem_unlink("/die") < 0 && errno != ENOENT)
			write(2, "Error: sem_unlink\n", 19);
	}
	return (dead);
}

/* @brief Simulation of sleeping time_to_sleep miliseconds */
/* @return 0 if everything went ok, 1 if any semaphore operation failed
or checked that a philosopher died  */
int	ft_sleep(const t_philo *philo, t_philo **philos)
{
	int		dead;
	int		philo_id;
	long	time;

	philo_id = philo->philo_id;
	time = ft_get_time();
	dead = ft_sem_wait(philo->printer);
	if (time < 0 || dead)
		ft_kill_philo(philos, philo->philo_id - 1, philo->die);
	dead += ft_check_dead(philo);
	if (dead)
		return (ft_sem_post(philo->printer), dead);
	printf("%ld ms %d is sleeping\n", time - philo->start_ms, philo_id);
	dead = ft_sem_post(philo->printer);
	if (ft_usleep(philo->table->t_to_sleep, philo))
	{
		if (sem_unlink("/die") < 0 && errno != ENOENT)
			write(2, "Error: sem_unlink\n", 19);
		dead = 1;
	}
	return (dead);
}
