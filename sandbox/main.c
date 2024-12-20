
#include <unistd.h>
#include <stdio.h>
int	main()
{
	printf("pwd:%s\n", getcwd(NULL, 0));
	return(0);
}
