#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>
#include <string.h>

typedef struct s_data
{
	pthread_mutex_t	lock;
	int	n;
	char	*msg;
}	t_data;

usleep();

void	*ft_print(void *arg)
{
	t_data	*data;
	struct timeval	tv;
	suseconds_t	t;

	gettimeofday(&tv, NULL);
	t = tv.tv_usec;
	data = (t_data *)arg;
	pthread_mutex_lock(&data->lock);
	printf("Thread %d created\n", data->n);
	while (data->n < 100)
	{
		printf("n = %d\n", data->n);
		data->n = data->n + 1;
	}
	gettimeofday(&tv, NULL);
	t = tv.tv_usec - t;
	printf("time = %ld ms\n", t);
	data->n = 1;
	pthread_mutex_unlock(&data->lock);
}

int main(void)
{
	t_data	*data;
	pthread_t	thread;
	char	*s;
	struct timeval	tv;
	suseconds_t	t;

	gettimeofday(&tv, NULL);
	t = tv.tv_usec;
	data = (t_data *)calloc(1, sizeof(t_data));
	pthread_mutex_init(&data->lock, NULL);
	data->msg = NULL;
	data->n = 0;
	if (pthread_create(&thread, NULL, ft_print, (void *)data) != 0)
		return (printf("Thread creation failed!\n"), 1);
	// pthread_join(thread ,NULL);
	if (pthread_create(&thread, NULL, ft_print, (void *)data) != 0)
		return (printf("Thread creation failed!\n"), 1);
	pthread_join(thread, NULL);
	free(data);
	gettimeofday(&tv, NULL);
	t = tv.tv_usec - t;
	printf("final time = %ld ms\n", t);
}

// int	main()
// {
// 	long	t = 214748364912312;
// 	char c = 1;
// 	int	i = 7;
// 	printf("%ld\n", t);
// 	// while (i >= 0)
// 	// {
// 	// 	printf("%d", (c >> i) & 1);
// 	// 	i--;
// 	// }
// 	// printf("\n");
// }