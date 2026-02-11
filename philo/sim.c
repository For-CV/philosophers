/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael-m <rafael-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 19:08:53 by rafael-m          #+#    #+#             */
/*   Updated: 2026/02/04 20:50:48 by rafael-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// Ejecuta la acción de dormir.  @return Devuelve 1 si ha muerto éste
// o algun otro filósofo ó 0 en caso de éxito.
static inline int	sleeping(const t_philo *philo)
{
	long	current_t;
	int		dead;

	current_t = get_time();
	dead = check_dead(philo, current_t);
	if (!dead)
	{
		print_action(philo, SLEEP);
		dead = ft_usleep(philo->t_to_sleep, philo);
	}
	return (dead);
}

// Ejecuta la acción de comer. @return Devuelve 1 si ha muerto éste
// o algun otro filósofo ó 0 en caso de éxito.
static inline int	eating(t_philo *philo)
{
	int		dead;
	int		ret;

	dead = check_dead(philo, get_time());
	if (!dead)
	{
		if (ft_mutex_lock(philo->last_meal_mtx))
			return (1);
		philo->last_meal_ms = get_time();
		ft_mutex_unlock(philo->last_meal_mtx);
		print_action(philo, EAT);
		dead = ft_usleep(philo->t_to_eat, philo);
	}
	ret = ft_mutex_unlock(&philo->forks[philo->fork2]);
	ret += ft_mutex_unlock(&philo->forks[philo->fork1]);
	return (dead + ret);
}

static bool	philo_actions(t_philo *philo, int i)
{
	if (sleeping(philo))
		return (true);
	if (thinking(philo))
		return (true);
	if (philo->n_to_eat && (i == philo->n_to_eat))
	{
		ft_mutex_lock(philo->finished_mtx);
		philo->finished = 1;
		ft_mutex_unlock(philo->finished_mtx);
		return (true);
	}
	return (false);
}

// Ya en cada hilo, el filósofo ejecuta las acciones pertinentes
void	*philo_sim(void *arg)
{
	t_philo	*philo;
	int		i;

	i = 0;
	philo = (t_philo *)arg;
	if (philo->n_phil == 1)
	{
		take_forks(philo, philo->fork1);
		ft_usleep(philo->t_to_die, philo);
		ft_mutex_unlock(&philo->forks[philo->fork1]);
		return (NULL);
	}
	if (philo->philo_id % 2 == 0)
		usleep(1000);
	while (!philo->n_to_eat || i <= (philo->n_to_eat + 1))
	{
		if (take_both_forks(philo))
			break ;
		if (eating(philo))
			break ;
		i++;
		if (philo_actions(philo, i))
			break ;
	}
	return (NULL);
}

//Inicia un hilo por filósofo pasándole la estructura tphilo correspondiente.
int	start_sim(t_philo *philos)
{
	pthread_t	monitoring;
	pthread_t	finished;

	if (set_time(philos))
		return (1);
	philos->threads = (pthread_t *)ft_calloc(philos->n_phil, sizeof(pthread_t));
	if (!philos->threads)
		return (1);
	if (pthread_create(&(finished), NULL, check_finished, (void *)philos))
		return (write(2, "Error: pthread_create\n", 22), 1);
	if (create_threads(philos))
		return (1);
	if (pthread_create(&(monitoring), NULL, ft_monitoring, (void *)philos))
		return (write(2, "Error: pthread_create\n", 22), 1);
	collect_philos(philos->threads, philos);
	ft_pthread_join(monitoring);
	ft_pthread_join(finished);
	return (0);
}
