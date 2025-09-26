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
	printf("size = %zu, INT_MAX = %u, UINTMAX = %ld\n", sizeof(long long), 1U, UINT_MAX / 1000);
}