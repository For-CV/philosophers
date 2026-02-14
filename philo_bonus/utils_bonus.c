/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael-m <rafael-m@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 18:29:12 by rafael-m          #+#    #+#             */
/*   Updated: 2025/12/05 19:38:55 by rafael-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

// Pequeño retardo inicial para escalonar a los filósofos según su orden
// de creación. @return 1 en caso de éxito, 0 si hay error.
int	wait_turn(const t_philo *philo)
{
	long	delay;

	if (philo->table->n_philos <= 1)
		return (0);
	delay = 0;
	if ((philo->philo_id % 2) == 0)
		delay = philo->table->t_to_eat / 2;
	else
		delay = 1;
	if (delay <= 0)
		return (0);
	return (ft_usleep(delay));
}

/* @brief Calloc implementation with error printing */
void	*ft_calloc(const size_t nmemb, const size_t size)
{
	void	*result;
	size_t	bytes;
	size_t	i;

	if (nmemb == 0 || size == 0)
	{
		result = malloc(0);
		return (result);
	}
	if (nmemb > SIZE_MAX / size)
		return (NULL);
	bytes = nmemb * size;
	result = malloc(bytes);
	if (!result)
		return (write(2, "Error: malloc\n", 14), NULL);
	i = 0;
	while (i < (bytes))
	{
		((char *)result)[i] = '\0';
		i++;
	}
	return (result);
}

/* @brief Strlen implementation, but more secure */
int	ft_strlen(const char *s)
{
	int	i;

	i = 0;
	if (!s)
		return (0);
	while (s[i])
		i++;
	return (i);
}

void	init_name(char *buf)
{
	buf[0] = '/';
	buf[1] = 'm';
	buf[2] = 'e';
	buf[3] = 'a';
	buf[4] = 'l';
	buf[5] = '_';
}
