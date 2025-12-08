/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wrappers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael-m <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 15:46:16 by rafael-m          #+#    #+#             */
/*   Updated: 2025/12/07 15:46:18 by rafael-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// pthread_mutex_lock wrapper, returns its errors plus 1 if !mutex.
int	ft_mutex_lock(pthread_mutex_t *mutex)
{
	int	ret;

	if (!mutex)
		return (1);
	ret = pthread_mutex_lock(mutex);
	if (ret)
		write(2, "Error: pthread_mutex_lock\n", 26);
	return (ret);
}

// pthread_mutex_unlock wrapper, returns its errors plus 2 if !mutex.
int	ft_mutex_unlock(pthread_mutex_t *mutex)
{
	int	ret;

	if (!mutex)
		return (2);
	ret = pthread_mutex_unlock(mutex);
	if (ret)
		write(2, "Error: pthread_mutex_unlock\n", 28);
	return (ret);
}

// pthread_mutex_destroy wrapper, returns its errors plus 1 if !mutex.
int	ft_mutex_destroy(pthread_mutex_t *mutex)
{
	int	ret;

	if (!mutex)
		return (1);
	ret = pthread_mutex_destroy(mutex);
	if (ret)
		write(2, "Error: pthread_mutex_destroy\n", 29);
	return (ret);
}

// pthread_join wrapper, returns its errors.
int	ft_pthread_join(pthread_t thread)
{
	int	ret;

	ret = pthread_join(thread, NULL);
	if (ret)
		write(2, "Error: pthread_join\n", 20);
	return (ret);
}
