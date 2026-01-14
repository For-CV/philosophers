/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wrappers_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael-m <rafael-m@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 18:29:12 by rafael-m          #+#    #+#             */
/*   Updated: 2025/12/05 18:29:12 by rafael-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/* sem_wait wrapper with error printing. @return sem_wait errno. */
int ft_sem_wait(sem_t *sem)
{
	int	error;

	if (!sem || sem == SEM_FAILED)
		return (write(2, "Error: sem_wait\n", 16), 1);
	error = sem_wait(sem);
	if (error)
	{
		write(2, "Error: sem_wait\n", 16);
		error = 1;
	}
	return (error);
}

/* sem_open wrapper with error printing. @return sem_open errno. */
sem_t *ft_sem_open(const char *name, int oflag, mode_t mode, unsigned int value)
{
	sem_t	*error;

	if (!name || !oflag)
		return (write(2, "Error: sem_open\n", 16), SEM_FAILED);
	if (mode || value)
		error = sem_open(name, oflag, mode, value);
	else
		error = sem_open(name, oflag);
	if (error == SEM_FAILED)
		write(2, "Error: sem_open\n", 16);
	return (error);
}

/* sem_post wrapper with error printing. @return sem_post errno. */
int ft_sem_post(sem_t *sem)
{
	int	error;

	if (!sem || sem == SEM_FAILED)
		return (write(2, "Error: sem_post\n", 16), 1);
	error = sem_post(sem);
	if (error)
		write(2, "Error: sem_post\n", 16);
	return (error);
}

/* sem_close wrapper with error printing. @return sem_close errno. */
int ft_sem_close(sem_t *sem)
{
	int	error;

	if (!sem || sem == SEM_FAILED)
		return (write(2, "Error: sem_close\n", 17), 1);
	error = sem_close(sem);
	if (error)
		write(2, "Error: sem_close\n", 17);
	return (error);
}

/* sem_unlink wrapper with error printing. @return sem_unlink errno. */
int	ft_sem_unlink(const char *name)
{
	int	error;

	if (!name)
		return (write(2, "Error: sem_unlink\n", 18), 1);
	error = sem_unlink(name);
	if (error)
		write(2, "Error: sem_unlink\n", 18);
	return (error);
}