#include "../src/philo.h"
#include "tests.h"

static void print_table(int fd, t_table *table)
{
    write(fd, "    n_philos: ", 14);
    ft_putnbr_fd(table->n_philos, fd);
    write(fd, "\n", 1);
    // Truncated for brevity, logic is verifying valid parse
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
    char *line;
    char *pipe_pos;
    char *expected_str;
    char *input_str;
    int expect_success;
    int result;

    if (!s)
        return (0);
    
    line = strdup(s);
    if (!line) return (1);

    pipe_pos = strchr(line, '|');
    if (!pipe_pos)
    {
        free(line);
        return (0);
    }

    *pipe_pos = '\0';
    expected_str = line;
    input_str = pipe_pos + 1;

    expect_success = (strcmp(expected_str, "PASS") == 0);

    // split_string modifies the string, so we use input_str directly (it's inside 'line' copy)
    argv = split_string(input_str, &argc);
    
    result = ft_parse(&table, argv);

    write(fd, "Test ", 5);
    ft_putnbr_fd(n, fd);
    write(fd, ": Input='", 9);
    // Reconstruct input for log (since strtok destroyed it)?
    // Or just trust previous log.
    // Actually strtok replaces spaces with \0.
    // We can't easily print it again unless we copied it before split.
    // For now, minimalist log.
    write(fd, s, ft_strlen(s)); 
    write(fd, "' -> ", 5);

    if (result == expect_success)
    {
        write(fd, "PASS\n", 5);
        if (result) print_table(fd, &table);
        free(line);
        free(argv);
        return (0);
    }
    else
    {
        write(fd, "FAIL (Expected ", 15);
        write(fd, expected_str, ft_strlen(expected_str));
        write(fd, ")\n", 2);
        free(line);
        free(argv);
        return (1);
    }
}
