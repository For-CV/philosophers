#ifndef TESTS_H
# define TESTS_H

#define RESET   "\033[0m"
#define RED     "\033[31m"      /* Rojo */
#define GREEN   "\033[32m"      /* Verde */
#define YELLOW  "\033[33m"      /* Amarillo */
#define BLUE    "\033[34m"      /* Azul */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */

/* Unitary */
int	ft_test_atoi();
int	ft_get_line(int fd, char s[100]);

#endif