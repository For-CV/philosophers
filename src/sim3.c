#include "philo.h"

/* Asigna el philo_id al flag *dead para que se pueda imprimir la muerte del
 filósofo */
void	ft_set_death(const t_philo *philo)
{
	pthread_mutex_lock(philo->dead_m);
	*(philo->dead) = philo->philo_id;
	pthread_mutex_unlock(philo->dead_m);
}

/* Crear e inicializar fin_philos. @return La dirección del array 
o NULL si falla malloc*/
static	t_philo **ft_init_fin_philos(const int n_philos)
{
	t_philo	**fin_philos;
	int		i;
	

	fin_philos = (t_philo **)ft_calloc(n_philos, sizeof(t_philo *));
	if (!fin_philos)
		return (NULL);
	i = 0;
	while (i < n_philos)
	{
		fin_philos[i] = NULL;
		i++;
	}
	return (fin_philos);
		
}
/* Comprueba si todos los filósofos han comido n_to_eat veces, y señala la 
muerte *dead = -1 para terminar la simulación. */
void	*ft_check_finished(void *arg)
{
	int	i;
	int	finished;
	t_philo	**fin_philos;
	t_philo **philos;

	philos = (t_philo **)arg;
	fin_philos = ft_init_fin_philos(philos[0]->n_philos);
	if (!fin_philos)
		return (NULL);
	i = 0;
	finished = 0;
	while (i < philos[0]->n_philos)
	{
		pthread_mutex_lock(philos[0]->dead_m);
		if (philos[0]->dead || finished == philos[0]->n_philos)
		{
			pthread_mutex_unlock(philos[0]->dead_m);
			break ;
		}
		pthread_mutex_unlock(philos[0]->dead_m);
		if (!fin_philos[i] && philos[i]->finished)
		{
			fin_philos[i] = philos[i];
			finished++;
		}
		if (finished == philos[0]->n_philos)
			break ;
		i++;
		if (i == philos[0]->n_philos)
			i = 0;
	}
	pthread_mutex_lock(philos[0]->dead_m);
	*(philos[0]->dead) = -1;
	pthread_mutex_unlock(philos[0]->dead_m);
	free(fin_philos);
	return (NULL);
}