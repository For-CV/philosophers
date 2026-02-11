/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim3.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael-m <rafael-m@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 19:08:53 by rafael-m          #+#    #+#             */
/*   Updated: 2026/01/15 23:22:24 by rafael-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* Comprueba 1 vez cada filósofo si ha muerto algún filósofo o si
 * todos han comido n_veces.
@return 1 cuando se cumple alguna de las dos condiciones. */
static int	check_once(const t_philo *philos)
{
	int	i;
	int	finished;

	i = 0;
	finished = 0;
	while (i < philos->n_phil)
	{
		ft_mutex_lock(philos->finished_mtx);
		if (philos[i].finished)
			finished++;
		ft_mutex_unlock(philos->finished_mtx);
		i++;
	}
	return (finished);
}

/* Comprueba si todos los filósofos han comido n_to_eat veces, y señala la 
muerte *dead = -1 para terminar la simulación. */
void	*check_finished(void *arg)
{
	int		finished;
	t_philo	*philos;

	philos = (t_philo *)arg;
	while (1)
	{
		finished = check_once(philos);
		if (finished == philos->n_phil)
			break ;
		ft_mutex_lock(philos->dead_m);
		if (*(philos->dead) && *(philos->dead) != -2)
		{
			ft_mutex_unlock(philos->dead_m);
			break ;
		}
		ft_mutex_unlock(philos->dead_m);
		usleep(100);
	}
	ft_mutex_lock(philos->dead_m);
	*(philos->dead) = -2;
	ft_mutex_unlock(philos->dead_m);
	return (NULL);
}

// Ejecuta la acción de coger tenedores.  @return Devuelve 1 si ha muerto éste
// o algun otro filósofo ó 0 en caso de éxito.
int	take_forks(const t_philo *philo, const int fork)
{
	long	current_t;
	int		dead;

	if (ft_mutex_lock(&philo->forks[fork]))
		return (1);
	current_t = get_time();
	dead = check_dead(philo, current_t);
	if (!dead)
		print_action(philo, FORK);
	return (dead);
}

// Ejecuta la acción de pensar (en caso de un número impar de filósofos,
// introduce un pequeño delay).  @return Devuelve 1 si ha muerto éste
// o algun otro filósofo ó 0 en caso de éxito.
int	thinking(const t_philo *philo)
{
	long	current_t;
	int		dead;

	current_t = get_time();
	dead = check_dead(philo, current_t);
	if (!dead)
	{
		print_action(philo, THINK);
		if (philo->n_phil % 2 != 0)
			dead = ft_usleep(philo->t_to_eat * 0.9, philo);
	}
	return (dead);
}
