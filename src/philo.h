#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>
# include <string.h>
# include <stdint.h>
# include <limits.h>

# define ERR_MSG "Arguments expected: number_of_philosophers, time_to_die,\
 time_to_eat, time_to_sleep [number_of_times_each_philosopher_must_eat]\n"

# define FORK 1
# define EAT 2
# define SLEEP 3
# define THINK 4

/* Table with the arguments passed to the program */
typedef struct	s_table
{
	int				n_philos;
	__useconds_t	t_to_die;
	__useconds_t	t_to_eat;
	__useconds_t	t_to_sleep;
	int				n_to_eat;
}	t_table;

/* Structure relevant to each philosopher */
typedef struct	s_philo
{
	int					philo_id;
	int					n_philos;
	__useconds_t		t_to_die;
	__useconds_t		t_to_eat;
	__useconds_t		t_to_sleep;
	int					n_to_eat;
	long				last_meal_ms;
	long				start_ms;
	int					*dead;
	int					fork1;
	int					fork2;
	pthread_mutex_t		**forks;
	pthread_mutex_t		*printer;
	pthread_mutex_t		*dead_mtx;
	pthread_mutex_t		*last_meal_mtx;
	pthread_t			*threads;
}	t_philo;

/* Estructura con los mutexes */
typedef struct s_mtxs
{
	pthread_mutex_t	*dead_mtx;
	pthread_mutex_t	*printer;
	pthread_mutex_t	*last_meal_mtx;
	pthread_mutex_t	**forks;
}	t_mtxs;

/* Utils and parsing */
void			*ft_calloc(const size_t nmemb, const size_t size);
int				ft_parse(t_table *table, char **argv);
int				ft_strlen(const char *s);
int				ft_special_atoi(const char *s);

/* Creating resources */
t_mtxs			*ft_init_mtxs(int n_philos);
int				ft_init_philos(t_philo **philos, const t_table *table, t_mtxs *mtxs, int *dead);
t_philo 		 **ft_create_philos(const t_table *table, t_mtxs *mtxs);

/* Time relative */
int				ft_set_time(t_philo **philos);
long			ft_get_time(void);
int				ft_usleep(const long ms, const t_philo *philo);

/* Freeing memory and liberating resources */
void			ft_free_mtxs(t_mtxs *mtxs, const int n_philos);
void			ft_free_philos(t_philo **philo);
void			ft_collect_philos(pthread_t *threads, t_philo **philos);

/* Simulation */
int				ft_start_sim(t_philo **philos);
void			ft_set_death(const t_philo *philo);
void			*ft_monitoring(void *arg);
void			ft_print_action(const t_philo *philo, const long current_t, const int action);
int				ft_check_dead(const t_philo *philo, const long current_t);
void 			*ft_philo(void *arg);
int				ft_create_threads(t_philo **philos);
int				ft_takefork(const t_philo *philo, const int fork);
int				ft_take_both_forks(const t_philo *philo);

#endif