#include <termios.h>
#include <stdio.h>
#include <unistd.h>

void	detect_key(void)
{
	char			buffer[2];

	printf("press ESC twice to exit program\n");
	while (1)
	{
		read(0, buffer, 1);
		if (buffer[0] == '\033')
		{
			read(0, buffer, 2);
			if (buffer[0] == '[')
			{
				if (buffer[1] == 'A')
					printf("arrow up was pressed\n");
				else if (buffer[1] == 'B')
					printf("arrow down was pressed\n");
				else if (buffer[1] == 'C')
					printf("arrow right was pressed\n");
				else if (buffer[1] == 'D')
					printf("arrow left was pressed\n");
				else
					printf("after ESCAPE: %c %c\n", buffer[0], buffer[1]);
			}
			else if (buffer[0] == '\033')
				break ;
		}
		else
			printf("key pressed: %c\n", buffer[0]);
	}
}

int	main()
{
	struct termios	termios_p;
	struct termios	termios_p_backup;
	int				ret;

	ret = tcgetattr(0, &termios_p);
	if (ret == -1)
		printf("error while getting terminal atributes\n");

	ret = tcgetattr(0, &termios_p_backup);
	if (ret == -1)
		printf("error while getting terminal atributes\n");

	termios_p.c_lflag &= ~(ICANON); // put terminal in non canonical mode
	termios_p.c_lflag &= ~(ECHO);	// disable echo
	ret = tcsetattr(0, 0, &termios_p);
	if (ret == -1)
		printf("error while setting atributes to terminal\n");

	detect_key();
	
	ret = tcsetattr(0, 0, &termios_p_backup); // restore terminal settings
	if (ret == -1)
		printf("error resetting terminal attributes");
}
