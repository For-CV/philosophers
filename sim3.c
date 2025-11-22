#include "philo.h"

/* Asigna el philo_id al flag *dead para que se pueda imprimir la muerte del
 filósofo */
void	ft_set_death(const t_philo *philo)
{
	pthread_mutex_lock(philo->dead_mtx);
	*(philo->dead) = philo->philo_id;
	pthread_mutex_unlock(philo->dead_mtx);
}

