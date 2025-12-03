#include "philo.h"

/* Crea e inicializa un mutex @return La dirección del mutex o NULL
 en caso de error (imprimiendo el mensaje de error)*/
static pthread_mutex_t	*ft_make_mtx(void)
{
	pthread_mutex_t *mtx;
	mtx = (pthread_mutex_t *)ft_calloc(1, sizeof(pthread_mutex_t));
	if (!mtx)
		return (NULL);
	pthread_mutex_init(mtx, NULL);
	return (mtx);
}

// Asigna fork1/fork2 en orden asimétrico para que los pares cojan primero
// el tenedor derecho y luego el izquierdo, y los impares lo hagan al revés.
// Esto mejora el convoy cuando n_philos es impar.
static void ft_assign_forks(t_philo *philo, int id)
{
	int	left;
	int	right;

	philo->philo_id = id;
	left = philo->philo_id - 1;
	right = philo->philo_id % philo->n_philos;
	if ((philo->philo_id % 2) == 0)
	{
		philo->fork1 = right;
		philo->fork2 = left;
	}
	else
	{
		philo->fork1 = left;
		philo->fork2 = right;
	}
}

/* Inicializa cada filósofo con los parámetros de la tabla de argumentos,
con el arrary de mutexes de tenedores, el que protege last_meal_ms y 
con el mutex para comprobar la muerte de los filósofos.
@return 1 en caso de éxito, 0 en caso de error. Libera t_table siempre. */
int	ft_init_philos(t_philo **philos, const t_table *table, t_mtxs *mtxs, int *dead)
{
	int	i;

	i = 0;
	while (i < table->n_philos)
	{
		philos[i]->n_philos = table->n_philos;
		philos[i]->t_to_die = table->t_to_die;
		philos[i]->t_to_eat = table->t_to_eat;
		philos[i]->t_to_sleep = table->t_to_sleep;
		philos[i]->n_to_eat = table->n_to_eat;
		philos[i]->printer = mtxs->printer;
		philos[i]->dead_m = mtxs->dead_m;
		philos[i]->last_meal_mtx = mtxs->last_meal_mtx;
		philos[i]->finished_mtx = mtxs->finished_mtx;
		philos[i]->dead = dead;
		philos[i]->forks = mtxs->forks;
		ft_assign_forks(philos[i], i + 1);
		i++;
		// printf("n_philos = %d\nphilo_id = %d\nt_to_die = %ld\nt_to_eat = %ld\nt_to_sleep = %ld\nn_to_eat = %ld\nforks = %p\nprinter = %p\ndead_mtx = %p\nlast_meal_mtx = %p\nfork1 = %d\nfork2 = %d\n\n", philos[i - 1]->n_philos, philos[i - 1]->philo_id, philos[i - 1]->t_to_die, philos[i - 1]->t_to_eat, philos[i - 1]->t_to_sleep, philos[i - 1]->n_to_eat, philos[i - 1]->forks, philos[i - 1]->printer, philos[i - 1]->dead_mtx, philos[i - 1]->last_meal_mtx, philos[i - 1]->fork1, philos[i - 1]->fork2);
	}
	return (1);
}

// @brief Crea las estructuras de datos que pasar a cada filósofo. No se
// imprime mensaje de error.
// @return El array de philósofos o NULL en caso de error, liberando todo.
t_philo  **ft_create_philos(const t_table *table, t_mtxs *mtxs)
{
	t_philo **philos;
	int		i;

	philos = (t_philo **)ft_calloc(table->n_philos, sizeof(t_philo *));
	if (!philos)
		return (ft_free_mtxs(mtxs, table->n_philos), NULL);
	i = 0;
	while (i < table->n_philos)
	{
		philos[i] = (t_philo *)ft_calloc(1, sizeof(t_philo));
		if (!philos[i])
		{
			ft_free_mtxs(mtxs, table->n_philos);
			while (--i >= 0)
				free(philos[i]);
			return (free(philos), NULL);
		}
		i++;
	}
	return (philos);
}


// Crea e inicia los mutexes.
// @return 0 en caso de éxito o 1 en caso de error.
int	ft_init_mtxs(t_mtxs *mtxs, const int n_philos)
{
	int	i;

	mtxs->dead_m = ft_make_mtx();
	mtxs->printer = ft_make_mtx();
	mtxs->last_meal_mtx = ft_make_mtx();
	mtxs->finished_mtx = ft_make_mtx();
	mtxs->forks = (pthread_mutex_t **)ft_calloc(n_philos, sizeof(pthread_mutex_t *));
	if (!mtxs->dead_m || !mtxs->printer || !mtxs->last_meal_mtx || !mtxs->forks || !mtxs->finished_mtx)
		return (ft_free_mtxs(mtxs, 0), 1);
	i = 0;
	while (i < n_philos)
		mtxs->forks[i++] = NULL;
	i = 0;
	while (i < n_philos)
	{
		mtxs->forks[i] = ft_make_mtx();
		if (!mtxs->forks[i])
		{
			ft_free_mtxs(mtxs, i);
			return (1);
		}
		i++;
	}
	return (0);
}