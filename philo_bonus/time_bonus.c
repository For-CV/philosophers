/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael-m <rafael-m@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 18:29:12 by rafael-m          #+#    #+#             */
/*   Updated: 2025/12/05 18:29:12 by rafael-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/* @brief Gets the current time since Epoch in miliseconds */
/* @return The (long)time in miliseconds or -1 on error*/
long	get_time(void)
{
	struct timeval	tv;
	long			time;

	if (gettimeofday(&tv, NULL))
	{
		write(2, "Error: gettimeofday\n", 19);
		return (-1);
	}
	time = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	return (time);
}

/* @brief Substitute of usleep, checking for philosophers deads every 200ms */
/* @return 0 if everything went ok, -1 if a philosopher died, 1 if this 
philosopher dies.*/
int	ft_usleep(const long ms)
{
	long	start;
	long	error;
	long	remaining_u;
	long	elapsed;

	start = get_time();
	while (1)
	{
		elapsed = get_time();
		if (elapsed < 0)
			return (-1);
		elapsed -= start;
		if (elapsed >= ms)
			break ;
		remaining_u = (ms - elapsed) * 1000;
		if (remaining_u > 500)
			error = usleep(500);
		else
			error = usleep(remaining_u);
		if (error)
			return (write(2, "Error: usleep\n", 14), 1);
	}
	return (0);
}
