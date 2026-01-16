/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael-m <rafael-m@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 19:41:00 by rafael-m          #+#    #+#             */
/*   Updated: 2026/01/14 10:22:29 by rafael-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// Asigna fork1/fork2 en orden asimétrico para que los pares cojan primero
// el tenedor derecho y luego el izquierdo, y los impares lo hagan al revés.
// Esto mejora el convoy cuando n_philos es impar.
static void	assign_forks(t_philo *philo, const int id)
{
	int	left;
	int	right;

	philo->philo_id = id;
	left = philo->philo_id - 1;
	right = philo->philo_id % philo->n_phil;
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
con el array de mutexes de tenedores, el que protege last_meal_ms y
con el mutex para comprobar la muerte de los filósofos.
@return 1 en caso de éxito, 0 en caso de error. Libera t_table siempre. */
int	init_philos(t_philo *philos, const t_table *table, const t_mtxs *mtxs,
					int *dead)
{
	int	i;

	i = 0;
	while (i < table->n_philos)
	{
		philos[i].n_phil = table->n_philos;
		philos[i].t_to_die = table->t_to_die;
		philos[i].t_to_eat = table->t_to_eat;
		philos[i].t_to_sleep = table->t_to_sleep;
		philos[i].n_to_eat = table->n_to_eat;
		philos[i].printer = mtxs->printer;
		philos[i].dead_m = mtxs->dead_m;
		philos[i].last_meal_mtx = mtxs->last_meal_mtx;
		philos[i].finished_mtx = mtxs->finished_mtx;
		philos[i].dead = dead;
		philos[i].forks = mtxs->forks;
		assign_forks(&philos[i], i + 1);
		i++;
	}
	return (1);
}

int	main(const int argc, char **argv)
{
	t_table			table;
	struct s_mtxs	mtxs;
	t_philo			*philos;
	int				dead;

	if (argc < 5 || argc > 6)
		return (write(2, ERR_MSG, 128), 1);
	if (!parser(&table, argv))
		return (1);
	if (init_mtxs(&mtxs, table.n_philos))
		return (1);
	dead = 0;
	philos = create_philos(&table, &mtxs);
	if (!philos || !init_philos(philos, &table, &mtxs, &dead))
		return (free_mtxs(&mtxs, table.n_philos), free_philos(philos), 1);
	if (start_sim(philos))
		return (free_mtxs(&mtxs, table.n_philos), free_philos(philos), 1);
	return (free_mtxs(&mtxs, table.n_philos), free_philos(philos), 0);
}
