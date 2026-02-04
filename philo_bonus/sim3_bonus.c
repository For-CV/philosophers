/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim3_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael-m <rafael-m@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 23:49:25 by rafael-m          #+#    #+#             */
/*   Updated: 2026/01/14 13:01:53 by rafael-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/* @brief Waits with waitpid for each philosopher.
 * Probably will need a kill option
 * for inevitables deadlocks (with sem_t *printer) */
/* @return = on success, 1 if waitpid failed. */
int	wait_philos(const int n_philos, const pid_t *pids)
{
	int	i;
	int	status;
	int	j;

	i = 0;
	while (i < n_philos)
	{
		if (waitpid(-1, &status, 0) != -1)
		{
			if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
			{
				j = 0;
				while (j < n_philos)
					kill(pids[j++], SIGKILL);
				return (0);
			}
		}
		else
			write(2, "Error: waitpid\n", 15);
		i++;
	}
	return (0);
}

static void	set_meal_time(t_philo *philo, long t, int *dead)
{
	ft_sem_wait(philo->meal_sem);
	philo->last_meal_ms = t;
	ft_sem_post(philo->meal_sem);
	*dead = ft_usleep(philo->table->t_to_eat);
}

static void	ft_kill_philo(const t_philo *philo, const long t)
{
	sem_wait(philo->die);
	sem_wait(philo->printer);
	printf("%ld ms %d died\n", t - philo->start_ms, philo->philo_id);
	exit(1);
}

static void	print_eating(t_philo *philo, const long t, int *dead)
{
	if (ft_sem_wait(philo->printer))
	{
		*dead = 1;
		return ;
	}
	printf("%ld ms %d is eating\n", t - philo->start_ms, philo->philo_id);
	*dead = ft_sem_post(philo->printer);
	set_meal_time(philo, t, dead);
	*dead += ft_sem_post(philo->forks);
	*dead += ft_sem_post(philo->forks);
	*dead += ft_sem_post(philo->seats);
}

/* @brief Simulation of eating time_to_eat milliseconds */
/* @return 0 if everything went ok. 1 if any semaphore operation failed,
 gettimeofday failed or time between lunches is bigger than time_to_die. */
int	eating(t_philo *philo)
{
	long	t;
	int		dead;

	t = get_time();
	if (t < 0)
		return (1);
	ft_sem_wait(philo->meal_sem);
	if (t - philo->last_meal_ms > philo->table->t_to_die)
	{
		ft_sem_post(philo->meal_sem);
		ft_kill_philo(philo, t);
	}
	ft_sem_post(philo->meal_sem);
	dead = 0;
	print_eating(philo, t, &dead);
	return (dead);
}
