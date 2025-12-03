#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "../src/philo.h"
#include "tests.h"

#define FDS 2

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

static int run_atoi_tests(void)
{
	int		fd[2];
	char	s[100];
	int		i;
	int		line;

	fd[0] = open("../tests_atoi.txt", O_RDONLY);
	if (fd[0] < 0)
		return (write(2, "Error: can't open tests.txt\n", 28), 1);
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
	close(fd[0]);
	close(fd[1]);
	return (0);
}

static int run_parse_tests(void)
{
	int		fd[2];
	char	s[100];
	int		i;
	int		line;

	fd[0] = open("../tests_parse.txt", O_RDONLY);
	if (fd[0] < 0)
		return (write(2, "Error: can't open tests_parse.txt\n", 34), 1);
	fd[1] = open("parse_log.txt", O_RDWR | O_CREAT, 0644);
	if (fd[1] < 0)
		return (write(2, "Error: can't open parse_log.txt\n", 32), 1);
	i = 1;
	line = 1;
	while (line)
	{
		line = ft_get_line(fd[0], s);
		if (line)
			ft_test_parse(i, s, fd[1]);
		i++;
	}
	close(fd[0]);
	close(fd[1]);
	return (0);
}

int	main()
{
	if (run_atoi_tests() != 0)
		return (1);
	if (run_parse_tests() != 0)
		return (1);
	return (0);
}
