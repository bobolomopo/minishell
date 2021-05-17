#include <unistd.h>
#include <stdio.h>

int	main(int argc, char **argv)
{
	int	n;
	int	i;

	i = -1;
	while (++i < argc)
		printf("argv[%d]: %s\n", i, argv[i]);
	
	n = 5;
	while (n--)
	{
		write(1, "ZzzzZzzz\n", 9);
		sleep(2);
	}
	return (42);
}
