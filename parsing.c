#include "philosophers.h"

int	ft_isspace(int	c)
{
	if (c == 32 || (c >= 9 && c <= 13))
		return (1);
	return (0);
}

static int	ft_skip_space(char *s)
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

long	ft_atoi(char *s)
{
	int					i;
	long				t;
	unsigned int		r;
	if (!s)
		return (-1);
	i = ft_skip_space(s);
	if (i < 0)
		return (0);
	r = 0;
	t = 0;
	while (s[i] >= '0' && s[i] <= '9')
	{
		t = t * 10 + (s[i] + '0');
		if (t > (UINT_MAX / 1000))
			return (-1);
		r = r * 10 + (s[i] - '0');
		i++;
	}
	if (i < ft_strlen(s))
		return (-1);
	return (r);
}

int	ft_parse(t_data *data, char **argv)
{
	unsigned int	t;

	data->n_to_eat = 0;
	data->n_philos = ft_atoi(argv[1]);
	if (data->n_philos < 0)
		return (write(2, "number_philo must be an int\n", 28), 0);
	t = ft_atoi(argv[2]);
	if (t < 0)
		return (write(2, "time_to_die must be an int\n", 27), 0);
	data->t_to_die = (__useconds_t)(t * 1000);
	t = ft_atoi(argv[3]);
	if (t < 0)
		return (write(2, "time_to_eat must be an int\n", 27), 0);
	data->t_to_eat = (__useconds_t)(t * 1000);
	t = ft_atoi(argv[4]);
	if (t < 0)
		return (write(2, "t_to_sleep must be an int\n", 26), 0);
	data->t_to_sleep = (__useconds_t)(t * 1000);
	if (argv[5])
	{
		data->n_to_eat = ft_atoi(argv[5]);
		if (data->n_to_eat < 0)
			return (write(2, "number_to_eat must be an int\n", 29), 0);
	}
	return (1);
}