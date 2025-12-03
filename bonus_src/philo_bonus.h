#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <sys/wait.h>
# include <string.h>
# include <stdint.h>
# include <limits.h>
# include <semaphore.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <errno.h>

# define ERR_MSG "Arguments expected: number_of_philosophers, time_to_die,\
 time_to_eat, time_to_sleep [number_of_times_each_philosopher_must_eat]\n"
# define FORK 1
# define EAT 2
# define SLEEP 3
# define THINK 4
# define MAX_PHILOS 200
# ifndef NAME_MAX
#  define NAME_MAX 255
# endif

/* Table with the arguments passed to the program */
typedef struct s_table
{
	int				n_philos;
	__useconds_t	t_to_die;
	__useconds_t	t_to_eat;
	__useconds_t	t_to_sleep;
	int				n_to_eat;
}	t_table;

/* Structure relevant to each fhilosopher */
typedef	struct s_philo
{
	int		philo_id;
	t_table	*table;
	long	last_meal_ms;
	long	start_ms;
	int		meals_eaten;
	sem_t	*forks;
	sem_t	*printer;
	sem_t	*die;
	sem_t	*seats;
}	t_philo;

/* Utils and parsing */

void	*ft_calloc(const size_t nmemb, const size_t size);
int		ft_special_atoi(const char *s);
int		ft_parse(t_table *table, char **argv);
int		ft_strlen(const char *s);
void    ft_putnbr(int n);

/* Fuctions wrappers */

int 	ft_sem_wait(sem_t *sem);
sem_t 	*ft_sem_open(const char *name, int oflag, mode_t mode, unsigned int value);
int 	ft_sem_post(sem_t *sem);
int 	ft_sem_close(sem_t *sem);
int		ft_sem_unlink(const char *name);

/* Freeing allocated memory and liberating resources */

void	ft_free_when_creating(t_philo **philos, sem_t *forks);
void	ft_free_philos(t_philo **philos);
void	ft_free_child(t_philo **philos);
void	ft_close_forks(sem_t *forks, const int child);

/* Time relative */

int		ft_usleep(long ms, const t_philo *philo);
long	ft_get_time(void);

/* Simulation */

int		ft_start_sim(t_philo **philos);
int		ft_wait_philos(const int n_philos);
int		ft_wait_turn(const t_philo *philo);
int		ft_takeforks(const t_philo *philo);
int		ft_check_dead(const t_philo *philo);
int		ft_sems_post(const t_philo *philo);
int		ft_sleep(const t_philo *philo, t_philo **philos);
void	ft_kill_philo(t_philo **philos, int n_philo, sem_t *die);
int		ft_eat(t_philo *philo);

#endif