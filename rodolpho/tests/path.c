#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
	struct stat buf;
	int	ret;
	
	char *str = getenv("PATH");
	printf("PATH: %s\n", str);

	ret = stat("/bin/ls", &buf);

	printf("stat returned %d\n", ret);

}

