#include "../bonus_src/philo_bonus.h"
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
	char	*line;
	char	*pipe_pos;
	char	*expected_str;
	char	*input_str;
	int		expected;
	int		result;

	if (!s)
		return (0);
	
	// Make a copy to modify
	line = strdup(s);
	if (!line) return (1);

	pipe_pos = strchr(line, '|');
	if (!pipe_pos)
	{
		free(line);
		return (0); // Skip malformed lines
	}

	*pipe_pos = '\0';
	expected_str = line;
	input_str = pipe_pos + 1;

	expected = atoi(expected_str);
	result = special_atoi(input_str);

	write(fd, "Test ", 5);
	ft_putnbr_fd(n, fd);
	write(fd, " Input='", 9);
	write(fd, input_str, ft_strlen(input_str));
	write(fd, " Expected=", 11);
	ft_putnbr_fd(expected, fd);
	write(fd, " Got=", 5);
	ft_putnbr_fd(result, fd);

	if (result == expected)
	{
		write(fd, " -> PASS\n", 9);
		free(line);
		return (0);
	}
	else
	{
		write(fd, " -> FAIL\n", 9);
		free(line);
		return (1);
	}
}
