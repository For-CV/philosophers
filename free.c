#include "philo.h"

// Recoge los filósofos una vez han terminado.
void	ft_collect_philos(pthread_t *threads, t_philo **philos)
{
	int		dead;
	int		n_philos;

	dead = 0;
	n_philos = (*philos)->n_philos;
	while (dead < n_philos)
	{
		pthread_join(threads[dead], NULL);
		dead++;
	}
}

// Libera y destruye los mutexes, libera y destruye n_philos forks.
void	ft_free_mtxs(t_mtxs *mtxs, int n_philos)
{
	int	i;

	if (!mtxs || n_philos == 0)
		return ;
	i = 0;
	while (i < n_philos)
	{
		if (mtxs->forks[i])
			pthread_mutex_destroy(mtxs->forks[i]);
		free(mtxs->forks[i]);
		i++;
	}
	free(mtxs->forks);
	if (mtxs->dead_mtx)
			pthread_mutex_destroy(mtxs->dead_mtx);
	free(mtxs->dead_mtx);
	if (mtxs->last_meal_mtx)
			pthread_mutex_destroy(mtxs->last_meal_mtx);
	free(mtxs->last_meal_mtx);
	if (mtxs->printer)
			pthread_mutex_destroy(mtxs->printer);
	free(mtxs->printer);
	free(mtxs);
}

// Libera las estructuras de cada filósofo y el array, incluidos
// los mutexes. El flag initiated es para saber si los mutexes
// printer y dead_mtx están iniciados con pthread_mute_init.
void	ft_free_philos(t_philo **philo)
{
	int		n_philos;
	int		i;

	if (!philo || !(*philo))
		return ;
	n_philos = philo[0]->n_philos;
	i = 0;
	if (philo[0]->threads)
		free(philo[0]->threads);
	while (i < n_philos)
	{
		free(philo[i]);
		i++;
	}
	free(philo);
}