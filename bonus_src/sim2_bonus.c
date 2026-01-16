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

/* @brief Checks if any philosopher has died and prints this philosopher's dead
 if true. */
/* @return -1 if another philosopher has died, 1 if this philosopher dies 
while checking dead, else 0 */
int	check_dead(const t_philo *philo)
{
	sem_t	*s;
	long	time;

	errno = 0;
	time = get_time();
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
/* @return 0 if everything went ok, 1 if any sem_wait error */
static int	ft_sems_wait(const t_philo *philo)
{
	int	ret;

	if (wait_sems(philo, &ret))
		return (ret);
	printf("%ld ms %d has taken a fork\n", get_time() - philo->start_ms,
		philo->philo_id);
	ft_sem_post(philo->printer);
	if (ft_sem_wait(philo->forks) < 0)
		return (ft_sem_post(philo->forks) + ft_sem_post(philo->seats) + 1);
	if (ft_sem_wait(philo->printer))
		return (1);
	printf("%ld ms %d has taken a fork\n", get_time() - philo->start_ms,
		philo->philo_id);
	ft_sem_post(philo->printer);
	if (sem_wait(philo->printer) < 0)
	{
		ft_sem_post(philo->forks);
		ft_sem_post(philo->forks);
		return (ft_sem_post(philo->seats), 1);
	}
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
	ret += ft_sem_post(philo->printer);
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
	dead = check_dead(philo);
	pthread_mutex_lock(&philo->meal_mtx);
	if (!dead && (start_t - philo->last_meal_ms <= philo->table->t_to_die))
	{
		pthread_mutex_unlock(&philo->meal_mtx);
		dead = ft_sem_post(philo->printer);
	}
	else
	{
		pthread_mutex_unlock(&philo->meal_mtx);
		ft_sem_post(philo->printer);
		if (sem_unlink("/die") < 0 && errno != ENOENT)
			write(2, "Error: sem_unlink\n", 19);
	}
	return (dead);
}

/* @brief Simulation of sleeping time_to_sleep milliseconds */
/* @return 0 if everything went ok, 1 if any semaphore operation failed
or checked that a philosopher died  */
int	sleeping(const t_philo *philo, t_philo **philos)
{
	int		dead;
	int		philo_id;
	long	time;

	philo_id = philo->philo_id;
	time = get_time();
	dead = ft_sem_wait(philo->printer);
	if (time < 0 || dead)
		kill_philo(philos, philo->philo_id - 1, philo->die);
	dead += check_dead(philo);
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
