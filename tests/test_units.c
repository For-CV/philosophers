#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "../philo.h"
#include "tests.h"

#define FDS 2

char	*ft_strjoin(char *s1, char *s2)
{
	char	*r;
	size_t	i;
	size_t	j;

	if (!s1 || !s2)
		return (NULL);
	r = (char *)ft_calloc(strlen(s1) + strlen(s2) + 1, sizeof(char));
	if (!r)
		return (write(2, "Error: malloc\n", 14), NULL);
	i = 0;	
	while (i < strlen(s1))
	{
		r[i] = s1[i];
		i++;
	}
	j = 0;
	while (j < strlen(s2))
	{
		r[i] = s2[j];
		i++;
		j++;
	}
	return (r);
}

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

// int	ft_open_logs()
int	ft_get_line(int fd, char s[100])
{
	ssize_t bytes;
	char c;
	int	i;


	bytes = 1;
	i = 0;
	while (bytes > 0)	
	{
		bytes = read(fd, &c, 1);
		if (bytes < 0)
			return (write(2, "Error: read\n", 12));
		if (bytes == 0)
			break ;
		if (c == '\n')
			break;
		s[i] = c;
		i++;
	}
	s[i] = '\0';
	if (i == 0)
		return (0);
	return (1);
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
	printf("s = %p; %s\n", s, s);
	t = ft_special_atoi(s);
	write(fd, "Result: ", 8);
	ft_putnbr_fd(t, fd);
	write(fd, "\n\n", 2);
	return (0);
}

int	main()
{
	int		fd[2];
	char	s[100];
	int		i;
	int		line;
	// char	*name;

	// (void)argc;
	fd[0] = open("../tests.txt", O_RDONLY);
	if (fd[0] < 0)
		return (write(2, "Error: can't open tests.txt\n", 28), 1);
	// name = ft_strjoin(argv[1], "atoi_log.txt");
	// if (!name)
	// 	return (1);
	// printf("name = %s\n", name);
	fd[1] = open("atoi_log.txt", O_RDWR | O_CREAT, 0644);
	if (fd[1] < 0)
		return (write(2, "Error: can't open atoi_log.txt\n", 31), 1);
	i = 1;
	line = 1;
	while (line)
	{
		line = ft_get_line(fd[0], s);
		if (line)
			ft_test_atoi(i, s, fd[1]);
		i++;
	}
	// free(name);
	close(fd[0]);
	close(fd[1]);
}