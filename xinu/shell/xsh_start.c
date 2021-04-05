/* xsh_ps.c - xsh_ps */

#include <xinu.h>
#include <stdio.h>
#include <string.h>

/*------------------------------------------------------------------------
 * xsh_ps - shell command to print the process table
 *------------------------------------------------------------------------
 */
shellcmd xsh_start(int nargs, char *args[])
{
	resume(pid_sensor[0]);
	resume(pid_sensor[1]);
	resume(pid_sensor[2]);
	
	scheduling_policy = 1;
	
	return 0;
}
