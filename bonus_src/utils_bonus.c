#include "philo_bonus.h"

/* Imprime un número a stdout */
void    ft_putnbr(int n)
{
        int     nbr;

        if (n == -2147483648)
        {
                write(1, "-2147483648", 11);
                return ;
        }
        if (n < 0)
        {
                write (1, "-", 1);
                n = -n;
        }
        nbr = n;
        if (nbr > 9)
                ft_putnbr(n / 10);
        nbr = (n % 10) + 48;
        write (1, &nbr, 1);
}

// Pequeño retardo inicial para escalonar a los filósofos según su orden
// de creación. @return 1 en caso de éxito, 0 si hay error.
int	ft_wait_turn(const t_philo *philo)
{
	long	delay;

	if (philo->table->n_philos <= 1)
		return (0);
	delay = 0;
	if ((philo->philo_id % 2) == 0)
		delay = philo->table->t_to_eat / 2;
	if ((philo->philo_id % 2) != 0)
		delay = philo->table->t_to_sleep / 2;
	if (delay <= 0)
		return (0);
	return (ft_usleep(delay, philo));
}

/* @brief Calloc implementation with error printing */
void *ft_calloc(const size_t nmemb, const size_t size)
{
	void	*result;
	size_t 	bytes;
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