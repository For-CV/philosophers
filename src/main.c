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

	/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael-m <rafael-m@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 19:41:00 by rafael-m          #+#    #+#             */
/*   Updated: 2025/12/06 22:56:02 by rafael-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(const int argc, char **argv)
{
	t_table			table;
	struct s_mtxs	mtxs;
	t_philo			*philos;
	int				dead;

	if (argc < 5 || argc > 6)
		return (write(2, ERR_MSG, 128), 1);
	if (!ft_parse(&table, argv))
		return (1);
	if (ft_init_mtxs(&mtxs, table.n_philos))
		return (1);
	dead = 0;
	philos = ft_create_philos(&table, &mtxs);
	if (!philos || !ft_init_philos(philos, &table, &mtxs, &dead))
		return (ft_free_mtxs(&mtxs, table.n_philos), ft_free_philos(philos), 1);
	if (ft_start_sim(philos))
		return (ft_free_mtxs(&mtxs, table.n_philos), ft_free_philos(philos), 1);
	return (ft_free_mtxs(&mtxs, table.n_philos), ft_free_philos(philos), 0);
}
