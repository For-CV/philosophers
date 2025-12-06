#include "philo_bonus.h"

/* @brief Waits with waitpid for each philosopher. Probably will need a kill option
for inevitables deadlocks (with sem_t *printer) */
/* @return = on success, 1 if waitpid failed. */
int	ft_wait_philos(const int n_philos)
{
	int	i;
	int	status;
	int	ret;

	i = 1;
	status = 0;
	ret = 0;
	while (i <= n_philos)
	{
		if (waitpid(-1, &status, 0) == -1)
		{
			ret++;
			write(2, "Error: waitpid\n", 15);
		}
		i++;
	}
	return (ret);
}

/* @brief Simulation of eating time_to_eat miliseconds */
/* @return 0 if everything went ok. 1 if any semaphore operation failed,
 gettimeofday failed or time between lunches is bigger than time_to_die.
  -1 if checked that another philosopher died  */
int	ft_eat(t_philo *philo)
{
	long	t;
	int		dead;

	if (ft_sem_wait(philo->printer))
		return (ft_sems_post(philo), 1);
	if (ft_check_dead(philo))
	{
		if (ft_sems_post(philo))
			return (1);
		return (-1);
	}
	t = ft_get_time();
	dead = 0;
	if (t - philo->last_meal_ms > philo->table->t_to_die)
	{
		printf("%ld ms %d died\n", t - philo->start_ms, philo->philo_id);
		if (sem_unlink("/die") < 0 && errno != ENOENT)
			write(2, "Error: sem_unlink\n", 18);
		dead = 1;
	}
	else
		printf("%ld ms %d is eating\n", t - philo->start_ms, philo->philo_id);
	dead += ft_sems_post(philo);
	if (dead)
		return (1);
	dead = ft_usleep(philo->table->t_to_eat, philo);
	return (philo->last_meal_ms = t, dead);
}