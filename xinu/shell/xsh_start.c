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
	resume(pid_sensor[1]);
	resume(pid_sensor[2]);
	resume(pid_actutator[0]);
	resume(pid_actutator[1]);

	scheduling_policy = 1;

	return 0;
}
