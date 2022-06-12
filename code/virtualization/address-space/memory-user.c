#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>

void	stream_through_array(char *array, unsigned int *idx, unsigned int length)
{
	*idx = *idx + 1;
	if (*idx == length) *idx = 0;
	array[*idx] = 42;
}

int		main(int argc, char *argv[]) 
{
	unsigned int	size, idx;
    int				running_time;
	char			*array;
	time_t			start, end;

	if (argc == 2)
	{
		size = atoi(argv[1])*1024*1024;
		running_time = -1;
	}
	else if (argc == 3)
	{
		size = atoi(argv[1])*1024*1024;
		running_time = atoi(argv[2]);
	}
	else
	{
		errno = EINVAL;
		perror("arguments must be one or two");
		return -1;
	}

	array = malloc(size);
	idx = 0;
	if (running_time < 0)
		while (1) stream_through_array(array, &idx, size);
	else
	{
		start = time(NULL);
		end = start;
		while (end - start < running_time)
		{
			stream_through_array(array, &idx, size);
			end = time(NULL);
		}
	}
	
}
