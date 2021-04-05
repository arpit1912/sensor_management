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
	kprintf("scheduling policy is working");
	scheduling_policy = 1;
	
	return 0;
}
