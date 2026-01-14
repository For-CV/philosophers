/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim3.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael-m <rafael-m@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 19:08:53 by rafael-m          #+#    #+#             */
/*   Updated: 2025/12/06 22:13:31 by rafael-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* Asigna el philo_id al flag *dead para que se pueda imprimir la muerte del
 filósofo */
// void	ft_set_death(const t_philo *philo)
// {
// 	ft_mutex_lock(philo->dead_m);
// 	*(philo->dead) = philo->philo_id;
// 	ft_mutex_unlock(philo->dead_m);
// }

/* Comprueba 1 vez cada filósofo si ha muerto algún filósofo o si todos han comido n_veces.
@return 1 cuando se cumple alguna de las dos condiciones. */
static int	ft_check_once(t_philo *philos)
{
	int	i;
	int	finished;

	i = 0;
	finished = 0;
	while (i < philos->n_philos)
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
void	*ft_check_finished(void *arg)
{
	int		finished;
	t_philo	*philos;

	philos = (t_philo *)arg;
	while (1)
	{
		finished = ft_check_once(philos);
		if (finished == philos->n_philos)
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
int	ft_takefork(const t_philo *philo, const int fork)
{
	long	current_t;
	int		dead;

	if (ft_mutex_lock(philo->forks[fork]))
		return (1);
	current_t = ft_get_time();
	dead = ft_check_dead(philo, current_t);
	if (!dead)
		ft_print_action(philo, FORK);
	return (dead);
}

// Ejecuta la acción de pensar (en caso de un número impar de filósofos,
// introduce un pequeño delay).  @return Devuelve 1 si ha muerto éste
// o algun otro filósofo ó 0 en caso de éxito.
int	ft_think(const t_philo *philo)
{
	long	current_t;
	int		dead;

	current_t = ft_get_time();
	dead = ft_check_dead(philo, current_t);
	if (!dead)
	{
		ft_print_action(philo, THINK);
		if (philo->n_philos % 2 != 0)
			dead = ft_usleep(philo->t_to_eat * 0.9, philo);
	}
	return (dead);
}
