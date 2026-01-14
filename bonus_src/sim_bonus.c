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

static void	*ft_monitor(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (1)
	{
		pthread_mutex_lock(&philo->meal_mtx);
		if (ft_get_time() - philo->last_meal_ms > philo->table->t_to_die)
		{
			sem_wait(philo->printer);
			printf("%ld ms %d died\n", ft_get_time() - philo->start_ms, philo->philo_id);
			sem_unlink("/die");
			exit(1);
		}
		pthread_mutex_unlock(&philo->meal_mtx);
		if (usleep(1000))
			break ;
	}
	return (NULL);
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
	printf("%ld ms %d died\n", ft_get_time() - philo->start_ms, philo->philo_id);
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
	philo->philo_id = n_philo + 1;
	if (pthread_create(&philo->monitor, NULL, ft_monitor, philo))
		exit(1);
	pthread_detach(philo->monitor);
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
			break ;
		if (ft_sleep(philo, philos))
			break ;
		if (ft_think(philo))
			break ;
	}
	pthread_mutex_destroy(&philo->meal_mtx);
	ft_free_child(philos);
	sem_close(die);
	if (sem_unlink("/die") < 0 && errno != ENOENT)
		write(2, "Error: sem_unlink\n", 19);
	exit(EXIT_SUCCESS);
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
	pid_t	*pids;

	if (!ft_set_time(philos))
		return (1);
	i = 0;
	n_philos = philos[i]->table->n_philos;
	pids = (pid_t *)ft_calloc(n_philos, sizeof(pid_t));
	if (!pids)
		return (write(2, "Error: malloc\n", 14), 1);
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
		pids[i] = pid;
		i++;
	}
	ft_wait_philos(i, pids);
	free(pids);
	return (0);
}