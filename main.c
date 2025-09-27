#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>
#include <string.h>
#include <stdatomic.h>
# include <limits.h>

int	main()
{
	struct  timeval *tv;
	suseconds_t	usec;
	time_t	sec;	
	
	tv = (struct timeval *)calloc(11, sizeof(struct timeval));
	if (!tv)
		return (1);
	gettimeofday(tv, NULL);
	usec = tv->tv_usec;
	sec = tv->tv_sec;
	printf("hola que tal\n");
	// write(1, "hola que tal\n", 13);
	gettimeofday(tv, NULL);
	printf("%ld\n", tv->tv_usec - usec);
}