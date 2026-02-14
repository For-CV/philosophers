/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafael-m <rafael-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 19:08:53 by rafael-m          #+#    #+#             */
/*   Updated: 2026/02/02 13:23:51 by rafael-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
# include <errno.h>
# include <stdbool.h>

# define ERR_MSG "Arguments expected: number_of_philosophers, time_to_die,\
 time_to_eat, time_to_sleep [number_of_times_each_philosopher_must_eat]\n"
# define NPHIL_ERR "number_philo must be a positive int < MAX_PHILOS\n"

# define FORK 1
# define EAT 2
# define SLEEP 3
# define THINK 4
# define MAX_PHILOS 200

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
	int				philo_id;
	int				n_phil;
	__useconds_t	t_to_die;
	__useconds_t	t_to_eat;
	__useconds_t	t_to_sleep;
	int				n_to_eat;
	long			last_meal_ms;
	long			start_ms;
	int				finished;
	int				*dead;
	int				fork1;
	int				fork2;
	pthread_mutex_t	*forks;
	pthread_mutex_t	*printer;
	pthread_mutex_t	*dead_m;
	pthread_mutex_t	*finished_mtx;
	pthread_mutex_t	*last_meal_mtx;
	pthread_t		*threads;
}	t_philo;

/* Structure with all mutexes */
typedef struct s_mtxs
{
	pthread_mutex_t	*dead_m;
	pthread_mutex_t	*printer;
	pthread_mutex_t	*last_meal_mtx;
	pthread_mutex_t	*finished_mtx;
	pthread_mutex_t	*forks;
}	t_mtxs;

/* Utils and parsing */

void	*ft_calloc(size_t nmemb, size_t size);
int		parser(t_table *table, char **argv);
int		ft_strlen(const char *s);
int		special_atoi(const char *s);
void	putlng_fd(long n, int fd);

/* Creating resources */

int		init_mtxs(t_mtxs *mtxs, int n_philos);
int		init_philos(t_philo *philos, const t_table *table,
			const t_mtxs *mtxs, int *dead);
t_philo	*create_philos(const t_table *table, t_mtxs *mtxs);

/* Time relative */

int		set_time(t_philo *philos);
long	get_time(void);
int		ft_usleep(long ms, const t_philo *philo);

/* Freeing memory and liberating resources */

void	free_mtxs(t_mtxs *mtxs, int n_philos);
void	free_philos(t_philo *philo);
void	collect_philos(const pthread_t *threads, const t_philo *philos);

/* Function wrappers */

int		ft_mutex_lock(pthread_mutex_t *mutex);
int		ft_mutex_unlock(pthread_mutex_t *mutex);
int		ft_mutex_destroy(pthread_mutex_t *mutex);
int		ft_pthread_join(pthread_t thread);

/* Simulation */

int		start_sim(t_philo *philos);
// void			ft_set_death(const t_philo *philo);
void	*ft_monitoring(void *arg);

int		print_action(const t_philo *philo, int action);
int		check_dead(const t_philo *philo, long current_t);
void	*philo_sim(void *arg);
int		create_threads(t_philo *philos);
int		take_forks(const t_philo *philo, int fork);
int		take_both_forks(const t_philo *philo);
void	*check_finished(void *arg);
int		thinking(const t_philo *philo);

#endif
