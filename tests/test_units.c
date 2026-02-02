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
	char		s[100];
	int		i;
	int		line;
    int     failures = 0;

	fd[0] = open("tests_atoi.txt", O_RDONLY);
	if (fd[0] < 0)
		return (write(2, "Error: can't open tests_atoi.txt\n", 33), 1);
	fd[1] = open("atoi_log.txt", O_RDWR | O_CREAT | O_TRUNC, 0644); // Added O_TRUNC
	if (fd[1] < 0)
		return (write(2, "Error: can't open atoi_log.txt\n", 31), 1);
	i = 1;
	line = 1;
	while (line)
	{
		line = ft_get_line(fd[0], s);
		if (line)
			failures += ft_test_atoi(i, s, fd[1]);
		i++;
	}
	close(fd[0]);
	close(fd[1]);
    
    if (failures > 0)
    {
        printf(RED "Unit Tests (Atoi): %d FAILURES. See tests/atoi_log.txt\n" RESET, failures);
        return (1);
    }
    printf(GREEN "Unit Tests (Atoi): ALL PASSED\n" RESET);
	return (0);
}

static int run_parse_tests(void)
{
	int		fd[2];
	char		s[100];
	int		i;
	int		line;
    int     failures = 0;

	fd[0] = open("tests_parse.txt", O_RDONLY);
	if (fd[0] < 0)
		return (write(2, "Error: can't open tests_parse.txt\n", 34), 1);
	fd[1] = open("parse_log.txt", O_RDWR | O_CREAT | O_TRUNC, 0644); // Added O_TRUNC
	if (fd[1] < 0)
		return (write(2, "Error: can't open parse_log.txt\n", 32), 1);
	i = 1;
	line = 1;
	while (line)
	{
		line = ft_get_line(fd[0], s);
		if (line)
			failures += ft_test_parse(i, s, fd[1]);
		i++;
	}
	close(fd[0]);
	close(fd[1]);

    if (failures > 0)
    {
        printf(RED "Unit Tests (Parse): %d FAILURES. See tests/parse_log.txt\n" RESET, failures);
        return (1);
    }
    printf(GREEN "Unit Tests (Parse): ALL PASSED\n" RESET);
	return (0);
}

int	main()
{
    int ret = 0;
	if (run_atoi_tests() != 0)
		ret = 1;
	if (run_parse_tests() != 0)
		ret = 1;
	return (ret);
}
