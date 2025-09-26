#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>
# include <string.h>
# include <stdint.h>
# include <limits.h>

# define ERR_MSG "Arguments expected: number_of_philosophers, time_to_die, time_to_eat, time_to_sleep [number_of_times_each_philosopher_must_eat]\n"
# define DEAD_MSG "All philosophers are dead\n"

typedef struct s_data
{
	pthread_mutex_t	global;
	int				n_philos;
	__useconds_t	t_to_die;
	__useconds_t	t_to_eat;
	__useconds_t	t_to_sleep;
	int				n_to_eat;
}	t_data;

typedef struct	s_philo
{
	int				n_philos;
	int				philo;
	__useconds_t	t_to_die;
	__useconds_t	t_to_eat;
	__useconds_t	t_to_sleep;
	int				n_to_eat;
	pthread_mutex_t	*forks;
	int				dead;
	int				n_dead;
	struct s_philo	*next;
}	t_philo;

void	*ft_calloc(size_t nmemb, size_t size);
void	ft_free_list(t_philo **start);
int		ft_atoi(char *s);
int		ft_strlen(char *s);
int		ft_parse(t_data *data, char **argv);

#endif