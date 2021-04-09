/* xsh_start.c - xsh_start */

#include <xinu.h>
#include <string.h>
#include <stdio.h>

static	uint32	parseval(char *);
extern	uint32	start;

/*------------------------------------------------------------------------
 * xsh_start - shell command to start the system
 *------------------------------------------------------------------------
 */
shellcmd xsh_start(int nargs, char *args[])
{
	printf("starting the process here : \n");

	resume(pid_sensor[0]);

	return 0;
}
