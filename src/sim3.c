#include "philo.h"

/* Asigna el philo_id al flag *dead para que se pueda imprimir la muerte del
 filósofo */
void	ft_set_death(const t_philo *philo)
{
	pthread_mutex_lock(philo->dead_m);
	*(philo->dead) = philo->philo_id;
	pthread_mutex_unlock(philo->dead_m);
}

/* Comprueba 1 vez cada filósofo si ha muerto algún filósofo o si todos han comido n_veces.
@return 1 cuando se cumple alguna de las dos condiciones. */
int	ft_check_once(t_philo **philos)
{
	int	i;
	int	finished;

	i = 0;
	finished = 0;
	while (i < philos[0]->n_philos)
	{
		pthread_mutex_lock(philos[0]->finished_mtx);
		if (philos[i]->finished)
			finished++;
		pthread_mutex_unlock(philos[0]->finished_mtx);
		i++;
	}
	return (finished);
}

/* Comprueba si todos los filósofos han comido n_to_eat veces, y señala la 
muerte *dead = -1 para terminar la simulación. */
void	*ft_check_finished(void *arg)
{
	int	finished;
	t_philo **philos;

	philos = (t_philo **)arg;
	finished = 0;
	while (1)
	{
		finished = ft_check_once(philos);
		if (finished == philos[0]->n_philos)
		{
			ft_putlng_fd(ft_get_time() - philos[0]->start_ms, 2);
			write(2, " finished!\n", 10);
			break ;
		}
		pthread_mutex_lock(philos[0]->dead_m);
		if (*(philos[0]->dead) && *(philos[0]->dead) != -2)
		{
			pthread_mutex_unlock(philos[0]->dead_m);
			break ;
		}
		pthread_mutex_unlock(philos[0]->dead_m);
		usleep(100);
	}
	pthread_mutex_lock(philos[0]->dead_m);
	*(philos[0]->dead) = -2;
	pthread_mutex_unlock(philos[0]->dead_m);
	return (NULL);
}