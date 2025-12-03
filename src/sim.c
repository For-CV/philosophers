#include "philo.h"

// Ejecuta la acción de pensar (en caso de un número impar de filósofos,
// introduce un pequeño delay).  @return Devuelve 1 si ha muerto éste
// o algun otro filósofo ó 0 en caso de éxito.
static inline int	ft_think(const t_philo *philo)
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

// Ejecuta la acción de dormir.  @return Devuelve 1 si ha muerto éste
// o algun otro filósofo ó 0 en caso de éxito.
static inline int	ft_sleep(const t_philo *philo)
{
	long	current_t;
	int		dead;

	current_t = ft_get_time();
	dead = ft_check_dead(philo, current_t);
	if (!dead)
	{
		ft_print_action(philo, SLEEP);
		dead = ft_usleep(philo->t_to_sleep, philo);
	}
	return (dead);
}

// Ejecuta la acción de comer. @return Devuelve 1 si ha muerto éste
// o algun otro filósofo ó 0 en caso de éxito.
static inline int	ft_eat(t_philo *philo)
{
	// long	current_t;
	int		dead;

	// current_t = ft_get_time();
	dead = ft_check_dead(philo, ft_get_time());
	if (!dead)
	{
		ft_print_action(philo, EAT);
		pthread_mutex_lock(philo->last_meal_mtx);
		philo->last_meal_ms = ft_get_time();
		pthread_mutex_unlock(philo->last_meal_mtx);
		dead = ft_usleep(philo->t_to_eat, philo);
	}
	pthread_mutex_unlock(philo->forks[philo->fork2]);
	pthread_mutex_unlock(philo->forks[philo->fork1]);
	return (dead);
}

// Ejecuta la acción de coger tenedores.  @return Devuelve 1 si ha muerto éste
// o algun otro filósofo ó 0 en caso de éxito.
int	ft_takefork(const t_philo *philo, const int fork)
{
	long	current_t;
	int		dead;

	pthread_mutex_lock(philo->forks[fork]);
	current_t = ft_get_time();
	dead = ft_check_dead(philo, current_t);	
	if (!dead)
		ft_print_action(philo, FORK);
	return (dead);
}

// Ya en cada hilo, el filósofo ejecuta las acciones pertinentes
void *ft_philo(void *arg)
{
	t_philo *philo;
	int		i;

	i = 0;
	philo = (t_philo *)arg;
	if (philo->philo_id % 2 == 0)
		usleep(1000);
	while (!philo->n_to_eat || i < philo->n_to_eat)
	{
		if (philo->n_to_eat && i == philo->n_to_eat)
		{
			pthread_mutex_lock(philo->finished_mtx);
			philo->finished = 1;
			pthread_mutex_unlock(philo->finished_mtx);
		}
		if (ft_take_both_forks(philo))
			break ;
		if (ft_eat(philo))
			break;
		if (ft_sleep(philo))
			break ;
		if (ft_think(philo))
			break ;
		i++;
	}
	return (NULL);
}

//Inicia un hilo por philósofo pasándole la estructura tphilo correspondiente.
int	ft_start_sim(t_philo **philos)
{
	pthread_t	monitoring;
	// pthread_t	finished;

	if (ft_set_time(philos))
		return (1);
	philos[0]->threads = (pthread_t *)ft_calloc((*philos)->n_philos, sizeof(pthread_t));
	if (!philos[0]->threads)
		return (1);
	// if (pthread_create(&(finished), NULL, ft_check_finished, (void *)philos))
	// 	return (write(2, "Error: pthread_create\n", 22), 1);
	if (ft_create_threads(philos))
		return (1);
	if (pthread_create(&(monitoring), NULL, ft_monitoring, (void *)philos))
		return (write(2, "Error: pthread_create\n", 22), 1);
	ft_collect_philos(philos[0]->threads, philos);
	pthread_join(monitoring, NULL);
	// pthread_join(finished, NULL);
	return (0);
}
