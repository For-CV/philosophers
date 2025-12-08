/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim_bonus.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael-m <rafael-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 15:56:08 by rafael-m          #+#    #+#             */
/*   Updated: 2025/12/08 19:48:11 by rafael-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/* Inicializa start_time y last_meal_t para todos los filósofos.
@return 1 en caso de éxito, 0 en caso de error. */
static int	ft_set_time(t_philo **philos)
{
	long	current_t;
	int		n_philos;
	int		i;

	n_philos = philos[0]->table->n_philos;
	i = 0;
	current_t = ft_get_time();
	if (current_t < 0)
		return (0);
	while (i < n_philos)
	{
		philos[i]->start_ms = current_t;
		philos[i]->last_meal_ms = current_t;
		i++;
	}
	return (1);
}

/* Imprime la muerte del filósofo, libera recursos y mata el proceso */
void	ft_kill_philo(t_philo **philos, int n_philo, sem_t *die)
{
	t_philo	*philo;
	int		status;

	status = 0;
	philo = philos[n_philo];
	philo->philo_id = n_philo + 1;
	status = sem_wait(philo->printer);
	write(2, "Error: sem_wait\n", 16);
	printf("%ld ms %d died\n", ft_get_time() - philo
		->start_ms, philo->philo_id);
	status += ft_sem_post(philo->printer);
	if (sem_unlink("/die") < 0 && errno != ENOENT)
	{
		status += errno;
		write(2, "Error: sem_unlink\n", 19);
	}
	ft_free_child(philos);
	status += ft_sem_close(die);
	exit(status);
}

/* @brief Prints when the philosopher starts thinking if no other 
philosofer died */
/* @return 0 on success, 1 if this philosopher died, -1 if another 
philosopher dies. */
static inline int	ft_think(t_philo *philo)
{
	long	t;
	int		dead;

	t = ft_get_time();
	if (ft_sem_wait(philo->printer))
		return (1);
	dead = ft_check_dead(philo);
	if (!dead)
		printf("%ld ms %d is thinking\n", t - philo->start_ms, philo->philo_id);
	if (ft_sem_post(philo->printer))
		return (1);
	return (dead);
}

/* @brief Executes philosophers actions: taking forks, eating, sleeping
and thinking */
/* @return to be decided upon */
static int	ft_philo(t_philo **philos, int n_philo, sem_t *die)
{
	t_philo	*philo;
	int		i;

	i = 0;
	philo = philos[n_philo];
	if (ft_wait_turn(philos[0]))
		i = -1;
	while (i >= 0)
	{
		if (i == INT_MAX)
			i = 0;
		if (ft_takeforks(philo))
			break ;
		if (ft_eat(philo))
			break ;
		i++;
		if (philo->table->n_to_eat > 0 && i >= philo->table->n_to_eat)
			ft_kill_philo(philos, n_philo, die);
		if (ft_sleep(philo, philos))
			break ;
		if (ft_think(philo))
			break ;
	}
	ft_exit_child(philos, die);
	return (1);
}

/* @brief Starts the simulation. Creates the pid_t array (probably should do it
in another function, merged with the ft_pid_error function), and forks once for
each philosopher, then waits for each proccess. Liberates all resources of the
father */
/* @return 0 if everything went ok, 1 for any failure */
int	ft_start_sim(t_philo **philos)
{
	int		i;
	int		n_philos;
	pid_t	pid;

	if (!ft_set_time(philos))
		return (1);
	i = 0;
	n_philos = philos[i]->table->n_philos;
	while (i < n_philos)
	{
		pid = fork();
		if (pid < 0)
		{
			write(2, "Error: fork\n", 12);
			break ;
		}
		if (pid == 0)
			ft_philo(philos, i, (*philos)->die);
		i++;
	}
	ft_wait_philos(i);
	return (0);
}
