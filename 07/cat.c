/* cat: minimal version */

#include <stdlib.h>
#include <unistd.h>
#define SIZE	512	/* arbitrary	*/

int main(int argc, char *argv[])
{
	char	buf[SIZE];
	int	n;
	
	while ((n = read(0, buf, sizeof buf)) > 0)
		write(1, buf, n);
	exit(0);
}
