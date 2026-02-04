/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim_bonus.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael-m <rafael-m@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 23:49:25 by rafael-m          #+#    #+#             */
/*   Updated: 2025/12/06 23:49:25 by rafael-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/* @brief Prints when the philosopher starts thinking if no other 
philosopher died */
/* @return 0 on success, 1 if this philosopher died, -1 if another 
philosopher dies. */
int	thinking(const t_philo *philo)
{
	long	t;
	int		dead;

	t = get_time();
	if (ft_sem_wait(philo->printer))
		return (1);
	dead = check_dead(philo);
	if (!dead)
		printf("%ld ms %d is thinking\n", t - philo->start_ms, philo->philo_id);
	if (ft_sem_post(philo->printer))
		return (1);
	return (dead);
}

static int	free_and_exit(t_philo *philos, sem_t *die, int ret)
{
	free_child(philos);
	sem_close(die);
	if (ret == 1)
		exit(1);
	exit(EXIT_SUCCESS);
}

/* @brief Executes philosophers actions: taking forks, eating, sleeping
and thinking */
/* @return to be decided upon */
static int	philo_sim(t_philo *philos, const int n_philo, sem_t *die)
{
	t_philo	*philo;
	int		i;
	int		ret;

	i = 0;
	ret = 0;
	philo = &philos[n_philo];
	philo->philo_id = n_philo + 1;
	philo->sim_active = true;
	if (pthread_create(&philo->monitor, NULL, ft_monitoring, philo))
		exit(1);
	if (wait_turn(philo))
		ret = 1;
	while (!ret && i >= 0)
	{
		if (philo_actions(philos, philo, &i, &ret))
			break ;
	}
	ft_sem_wait(philo->meal_sem);
	philo->sim_active = false;
	ft_sem_post(philo->meal_sem);
	if (pthread_join(philo->monitor, NULL))
		write(2, "Error: pthread_join\n", 20);
	return (free_and_exit(philos, die, ret));
}

static bool	fork_philos(t_philo *philos, pid_t *pids, int i)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		write(2, "Error: fork\n", 12);
		return (true);
	}
	if (pid == 0)
		philo_sim(philos, i, philos->die);
	pids[i] = pid;
	return (false);
}

/* Starts the simulation. Creates the pid_t array (probably should do it
in another function, merged with the ft_pid_error function), and forks once for
each philosopher, then waits for each process. Liberates all resources of the
father */
/* RETURN: 0 if everything went ok, 1 for any failure */
int	start_sim(t_philo *philos)
{
	int		i;
	int		n_philos;
	pid_t	*pids;

	if (!set_time(philos))
		return (1);
	i = 0;
	n_philos = philos[i].table->n_philos;
	pids = (pid_t *)ft_calloc(n_philos, sizeof(pid_t));
	if (!pids)
		return (write(2, "Error: malloc\n", 14), 1);
	while (i < n_philos)
	{
		if (fork_philos(philos, pids, i))
			break ;
		i++;
	}
	wait_philos(i, pids);
	free(pids);
	return (0);
}
