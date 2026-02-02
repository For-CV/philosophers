/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael-m <rafael-m@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 19:08:53 by rafael-m          #+#    #+#             */
/*   Updated: 2025/12/06 22:36:26 by rafael-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* Imprime la acción pertinente, el id del filósofo que la realiza */
int	print_action(const t_philo *philo, const int action)
{
	int		is_dead;
	long	current_t;

	is_dead = ft_mutex_lock(philo->dead_m);
	is_dead += *(philo->dead);
	is_dead += ft_mutex_lock(philo->printer);
	if (is_dead)
	{
		ft_mutex_unlock(philo->printer);
		return (ft_mutex_unlock(philo->dead_m), 1);
	}
	current_t = get_time();
	putlng_fd(current_t - philo->start_ms, 1);
	write(1, " ms ", 4);
	putlng_fd((long)philo->philo_id, 1);
	if (action == FORK)
		write(1, " has taken a fork\n", 18);
	else if (action == EAT)
		write(1, " is eating\n", 11);
	else if (action == THINK)
		write(1, " is thinking\n", 13);
	else if (action == SLEEP)
		write(1, " is sleeping\n", 13);
	ft_mutex_unlock(philo->printer);
	return (ft_mutex_unlock(philo->dead_m), 0);
}

/* Comprueba antes de imprimir una acción que no se ha superadp t_to_die
 o que no haya muerto algún filósofo. @return 1 Si hay alguna muerte, 0
 en caso contrario */
int	check_dead(const t_philo *philo, const long current_t)
{
	int	dead;

	if (current_t < 0)
	{
		ft_mutex_lock(philo->dead_m);
		*(philo->dead) = philo->philo_id;
		ft_mutex_unlock(philo->dead_m);
		return (1);
	}
	dead = ft_mutex_lock(philo->dead_m);
	dead += *(philo->dead);
	if (dead)
	{
		ft_mutex_unlock(philo->dead_m);
		return (1);
	}
	dead = ft_mutex_lock(philo->last_meal_mtx);
	if (philo->t_to_die < (current_t - philo->last_meal_ms) || dead)
	{
		ft_mutex_unlock(philo->last_meal_mtx);
		return (ft_mutex_unlock(philo->dead_m), 1);
	}
	dead = ft_mutex_unlock(philo->last_meal_mtx);
	dead += ft_mutex_unlock(philo->dead_m);
	return (dead);
}

/* Crea los hilos de cada filósofo @return 0 en caso de éxito,
1 si falla pthread_create*/
int	create_threads(t_philo *philos)
{
	int	i;

	i = 0;
	while (i < philos->n_phil)
	{
		if (pthread_create(&(philos->threads[i]), NULL, philo_sim,
				(void *) &philos[i]))
		{
			philos->n_phil = i;
			ft_mutex_lock(philos->dead_m);
			*(philos->dead) = 1;
			ft_mutex_unlock(philos->dead_m);
			collect_philos(philos->threads, philos);
			write(2, "Error: pthread_create\n", 22);
			return (1);
		}
		i++;
	}
	return (0);
}

/* Coger ambos tenedores. @return 1 si se comprueba alguna muerte, 0 en
caso de éxito */
int	take_both_forks(const t_philo *philo)
{
	if (take_forks(philo, philo->fork1))
	{
		ft_mutex_unlock(philo->forks[philo->fork1]);
		return (1);
	}
	if (take_forks(philo, philo->fork2))
	{
		ft_mutex_unlock(philo->forks[philo->fork1]);
		ft_mutex_unlock(philo->forks[philo->fork2]);
		return (1);
	}
	return (0);
}
