/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael-m <rafael-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 23:49:25 by rafael-m          #+#    #+#             */
/*   Updated: 2026/02/02 13:04:16 by rafael-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <sys/wait.h>
# include <signal.h>
# include <pthread.h>
# include <string.h>
# include <stdint.h>
# include <limits.h>
# include <semaphore.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <errno.h>
# include <stdbool.h>

# define ERR_MSG "Arguments expected: number_of_philosophers | time_to_die,\
 time_to_eat | time_to_sleep | [number_of_times_each_philosopher_must_eat]\n"
# define ERR "number_philo must be a positive int < 100000\n"
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

/* Structure relevant to each philosopher */
typedef struct s_philo
{
	sem_t			*forks;
	sem_t			*printer;
	sem_t			*die;
	sem_t			*seats;
	t_table			*table;
	long			last_meal_ms;
	long			start_ms;
	int				meals_eaten;
	int				philo_id;
	pthread_t		monitor;
	sem_t			*meal_sem;
	bool			sim_active;
}	t_philo;

/* Utils and parsing */

void	*ft_calloc(size_t nmemb, size_t size);
int		special_atoi(const char *s);
int		parser(t_table *table, char **argv);
int		ft_strlen(const char *s);
void	ft_putnbr(int n);
void	init_name(char *buf);

/* Functions wrappers */

int		ft_sem_wait(sem_t *sem);
sem_t	*ft_sem_open(const char *name, int oflag, mode_t mode,
			unsigned int value);
int		ft_sem_post(sem_t *sem);
int		ft_sem_close(sem_t *sem);
int		ft_sem_unlink(const char *name);

/* Freeing allocated memory and liberating resources */

void	free_when_creating(t_philo *philos, sem_t *forks);
void	free_philos(t_philo *philos);
void	free_child(t_philo *philos);
void	close_forks(sem_t *forks, int child);

/* Time relative */

int		ft_usleep(long ms);
long	get_time(void);

/* Simulation */

int		start_sim(t_philo *philos);
int		wait_philos(int n_philos, const pid_t *pids);
int		wait_turn(const t_philo *philo);
int		take_forks(t_philo *philo);
int		check_dead(const t_philo *philo);
int		ft_sems_post(const t_philo *philo);
int		sleeping(const t_philo *philo, t_philo *philos);
int		eating(t_philo *philo);
int		set_time(t_philo *philos);
bool	philo_actions(t_philo *philos, t_philo *philo, int *i, int *ret);
void	*ft_monitoring(void *arg);
int		thinking(const t_philo *philo);

#endif
