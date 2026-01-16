/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael-m <rafael-m@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 23:49:25 by rafael-m          #+#    #+#             */
/*   Updated: 2026/01/14 12:36:21 by rafael-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	ft_isspace(const int c)
{
	if (c == 32 || (c >= 9 && c <= 13))
		return (1);
	return (0);
}

static int	skip_space(const char *s)
{
	int	i;
	int	sign;

	sign = 1;
	i = 0;
	while (ft_isspace(s[i]))
		i++;
	while (s[i] == '-' || s[i] == '+')
	{
		if (s[i] == '-')
			sign = -sign;
		i++;
	}
	return (i * sign);
}

int	special_atoi(const char *s)
{
	int	i;
	int	r;
	int	digit;

	if (!s)
		return (-1);
	i = skip_space(s);
	if (i < 0)
		return (-1);
	r = 0;
	while (s[i] >= '0' && s[i] <= '9')
	{
		digit = s[i] - '0';
		if (r > (INT_MAX - digit) / 10)
			return (-1);
		r = r * 10 + digit;
		i++;
	}
	if (i < ft_strlen(s))
		return (-1);
	return (r);
}

int	parser(t_table *table, char **argv)
{
	int	t;

	table->n_to_eat = 0;
	table->n_philos = special_atoi(argv[1]);
	if (table->n_philos <= 0 || table->n_philos > MAX_PHILOS)
		return (write(2, ERR, 45), 0);
	t = special_atoi(argv[2]);
	if (t <= 0)
		return (write(2, "time_to_die must be a positive int\n", 35), 0);
	table->t_to_die = (__useconds_t)t;
	t = special_atoi(argv[3]);
	if (t <= 0)
		return (write(2, "time_to_eat must be a positive int\n", 35), 0);
	table->t_to_eat = (__useconds_t)t;
	t = special_atoi(argv[4]);
	if (t <= 0)
		return (write(2, "t_to_sleep must be a positive int\n", 34), 0);
	table->t_to_sleep = (__useconds_t)t;
	if (argv[5])
	{
		table->n_to_eat = special_atoi(argv[5]);
		if (table->n_to_eat < 0)
			return (write(2, "number_to_eat must be an int\n", 29), 0);
	}
	return (1);
}
