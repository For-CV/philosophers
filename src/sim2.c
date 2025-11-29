#include "philo.h"

// Pequeño retardo inicial para escalonar a los filósofos según su orden
// de creación.
int	ft_wait_turn(const t_philo *philo)
{
	long	delay;

	if (philo->n_philos <= 1)
		return (0);
	delay = 0;
	if ((philo->philo_id % 2) == 0)
		delay = philo->t_to_eat / 2;
	if ((philo->philo_id % 2) != 0)
		delay = philo->t_to_sleep / 2;
	if (delay <= 0)
		return (0);
	return (ft_usleep(delay, philo));
}

/* Imprime la acción pertinente, el id del filósofo que la realiza */
void	ft_print_action(const t_philo *philo, const long current_t, const int action)
{
	int	is_dead;

	pthread_mutex_lock(philo->printer);
	pthread_mutex_lock(philo->dead_mtx);
	is_dead = *(philo->dead);
	pthread_mutex_unlock(philo->dead_mtx);
	if (!is_dead)
	{
		if (action == FORK)
			printf("%ld ms %d has taken a fork\n", current_t - philo->start_ms, philo->philo_id);
		else if (action == EAT)
			printf("%ld ms %d is eating\n", current_t - philo->start_ms, philo->philo_id);
		else if (action == THINK)
			printf("%ld ms %d is thinking\n", current_t - philo->start_ms, philo->philo_id);
		else if (action == SLEEP)
			printf("%ld ms %d is sleeping\n", current_t - philo->start_ms, philo->philo_id);
	}
	pthread_mutex_unlock(philo->printer);
}

/* Comprueba antes de imprimir una acción que no se ha superadp t_to_die
 o que no haya muerto algún filósofo. @return 1 Si hay alguna muerte, 0
 en caso contrario */
 int	ft_check_dead(const t_philo *philo, const long current_t)
 {
	pthread_mutex_lock(philo->dead_mtx);
	if (current_t < 0)
		return (pthread_mutex_unlock(philo->dead_mtx), ft_set_death(philo), 1);
	pthread_mutex_lock(philo->last_meal_mtx);
	if (philo->t_to_die < (current_t - philo->last_meal_ms) && *(philo->dead))
	{
		pthread_mutex_unlock(philo->last_meal_mtx);
		if (*(philo->dead))
			return (pthread_mutex_unlock(philo->dead_mtx), 1);
		pthread_mutex_unlock(philo->dead_mtx);
		ft_set_death(philo);
		return (1);
	}
	pthread_mutex_unlock(philo->last_meal_mtx);
	pthread_mutex_unlock(philo->dead_mtx);
	return (0);
 }

 /* Crea los hilos de cada filósofo @return 0 en caso de éxito,
  1 si falla pthread_create*/
  int	ft_create_threads(t_philo **philos)
  {
	int	i;

	i = 0;
	while (i < (*philos)->n_philos)
	{
		if (pthread_create(&(philos[0]->threads[i]), NULL, ft_philo, (void *)philos[i]))
		{
			philos[0]->n_philos = i;
			pthread_mutex_lock(philos[0]->dead_mtx);
			*(philos[0]->dead) = 1;
			pthread_mutex_unlock(philos[0]->dead_mtx);
			ft_collect_philos(philos[0]->threads, philos);
			philos[0]->n_philos = philos[1]->n_philos;
			write(2, "Error: pthread_create\n", 22);
			return (1);
		}
		i++;
	}
	return (0);
  }

  /* Coger ambos tenedores. @return 1 si se comprueba alguna muerte, 0 en
  caso de éxito */
  int	ft_take_both_forks(const t_philo *philo)
  {
	if (ft_takefork(philo, philo->fork1))
	{
		pthread_mutex_unlock(philo->forks[philo->fork1]);
		return (1);
	}
	if (ft_takefork(philo, philo->fork2))
	{
		pthread_mutex_unlock(philo->forks[philo->fork1]);
		pthread_mutex_unlock(philo->forks[philo->fork2]);
		return (1);;
	}
	return (0);
  }