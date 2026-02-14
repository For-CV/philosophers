/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael-m <rafael-m@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 19:08:53 by rafael-m          #+#    #+#             */
/*   Updated: 2025/12/06 23:20:37 by rafael-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* Crea e inicializa un mutex @return La dirección del mutex o NULL
 en caso de error (imprimiendo el mensaje de error)*/
static pthread_mutex_t	*make_mtx(void)
{
	int				ret;
	pthread_mutex_t	*mtx;

	mtx = (pthread_mutex_t *)ft_calloc(1, sizeof(pthread_mutex_t));
	if (!mtx)
		return (NULL);
	pthread_mutex_init(mtx, NULL);
	ret = pthread_mutex_lock(mtx);
	if (ret)
	{
		if (ret == EINVAL)
			write(2, "Error: pthread_mutex_init\n", 26);
		else
			write(2, "Error: pthread_mutex_lock\n", 26);
		return (NULL);
	}
	if (ft_mutex_unlock(mtx))
	{
		ft_mutex_destroy(mtx);
		free(mtx);
		return (NULL);
	}
	return (mtx);
}

// @brief Crea las estructuras de datos que pasar a cada filósofo. No se
// imprime mensaje de error.
// @return El array de filósofos o NULL en caso de error, liberando todo.
t_philo	*create_philos(const t_table *table, t_mtxs *mtxs)
{
	t_philo	*philos;

	philos = (t_philo *)ft_calloc(table->n_philos, sizeof(t_philo));
	if (!philos)
		return (free_mtxs(mtxs, table->n_philos), NULL);
	return (philos);
}

static void	create_mtxs(t_mtxs *mtxs, const int n_philos)
{
	mtxs->dead_m = make_mtx();
	mtxs->printer = make_mtx();
	mtxs->last_meal_mtx = make_mtx();
	mtxs->finished_mtx = make_mtx();
	mtxs->forks = (pthread_mutex_t *) ft_calloc(
			n_philos, sizeof(pthread_mutex_t));
}

// Crea e inicia los mutexes.
// @return 0 en caso de éxito ó 1 en caso de error.
int	init_mtxs(t_mtxs *mtxs, const int n_philos)
{
	int	i;

	create_mtxs(mtxs, n_philos);
	if (!mtxs->dead_m || !mtxs->printer || !mtxs->last_meal_mtx || !mtxs->forks
		|| !mtxs->finished_mtx)
		return (free_mtxs(mtxs, 0), 1);
	i = 0;
	while (i < n_philos)
	{
		if (pthread_mutex_init(&mtxs->forks[i], NULL))
		{
			free_mtxs(mtxs, i);
			return (1);
		}
		i++;
	}
	return (0);
}
