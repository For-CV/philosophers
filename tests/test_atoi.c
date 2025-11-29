#include "../src/philo.h"
#include "tests.h"

void    ft_putnbr_fd(int n, int fd)
{
	int	nbr;

	if (n == -2147483648)
	{
		write(fd, "-2147483648", 11);
		return ;
	}
	if (n < 0)
	{
		write (fd, "-", 1);
		n = -n;
	}
	nbr = n;
	if (nbr > 9)
		ft_putnbr_fd(n / 10, fd);
	nbr = (n % 10) + 48;
	write (fd, &nbr, 1);
}

int	ft_test_atoi(const int n, const char *s, const int fd)
{
	int	t;

	if (!s)
		return (1);
	write(fd, "Atoi Test ", 10);
	ft_putnbr_fd(n, fd);
	write(fd, ": ", 2);
	write(fd, s, strlen(s));
	write(fd, ": \n", 3);
	t = atoi(s);
	write(fd, "Expected: ", 10);
	if (t >= 0 && atol(s) <= INT_MAX && atol(s) >= INT_MIN)
		ft_putnbr_fd(t, fd);
	else
		write(fd, "-1", 2);
	write(fd, "\n", 1);
	t = ft_special_atoi(s);
	write(fd, "Result: ", 8);
	ft_putnbr_fd(t, fd);
	write(fd, "\n\n", 2);
	return (0);
}