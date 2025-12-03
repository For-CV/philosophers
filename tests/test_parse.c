#include "../src/philo.h"
#include "tests.h"

static void print_table(int fd, t_table *table)
{
    write(fd, "n_philos: ", 10);
    ft_putnbr_fd(table->n_philos, fd);
    write(fd, "\n", 1);
    write(fd, "t_to_die: ", 10);
    ft_putlng_fd(table->t_to_die, fd);
    write(fd, "\n", 1);
    write(fd, "t_to_eat: ", 10);
    ft_putlng_fd(table->t_to_eat, fd);
    write(fd, "\n", 1);
    write(fd, "t_to_sleep: ", 12);
    ft_putlng_fd(table->t_to_sleep, fd);
    write(fd, "\n", 1);
    write(fd, "n_to_eat: ", 10);
    ft_putnbr_fd(table->n_to_eat, fd);
    write(fd, "\n", 1);
}

static char **split_string(char *str, int *argc)
{
    char **argv = malloc(sizeof(char *) * 10);
    int i = 0;
    char *p;

    argv[i++] = "./philo"; // Dummy program name
    p = strtok(str, " ");
    while (p != NULL)
    {
        argv[i++] = p;
        p = strtok(NULL, " ");
    }
    *argc = i;
    argv[i] = NULL;
    return argv;
}

int ft_test_parse(const int n, const char *s, const int fd)
{
    t_table table;
    int argc;
    char **argv;
    char *s_copy;
    int result;

    if (!s)
        return (1);
    
    s_copy = strdup(s);

    write(fd, "Parse Test ", 11);
    ft_putnbr_fd(n, fd);
    write(fd, ": ", 2);
    write(fd, s, strlen(s));
    write(fd, "\n", 1);

    argv = split_string(s_copy, &argc);
    
    result = ft_parse(&table, argv);

    write(fd, "Result: ", 8);
    if (result)
    {
        write(fd, "SUCCESS\n", 8);
        print_table(fd, &table);
    }
    else
    {
        write(fd, "FAILURE\n", 8);
    }
    write(fd, "\n", 1);
    free(s_copy);
    free(argv);
    return (0);
}