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
	int Actuator1Start = 0, Actuator2Start = 0;
	int ValidArg1 = 0, ValidArg2 = 0;
	if (nargs == 2 && strncmp(args[1], "--help", 7) == 0) {

		printf("Use: %s\n\n", args[0]);
		printf("Description:\n");
		printf("\tStart the Actuators\n");
		printf("Options:\n");
		printf("\t--help\t display this help and exit\n");
		return 0;
	}
	else if (nargs == 4){

		if(strncmp(args[1],"0",1) == 0 || strncmp(args[1],"1",1) == 0){
			ValidArg1 = 1;
		}

		if(strncmp(args[2],"0",1)== 0 || strncmp(args[2],"1",1) == 0){
			ValidArg2 = 1;
		}

		if( ValidArg1 == 1 && ValidArg2 == 1){
			Actuator1Start = atoi(args[1]);
			Actuator2Start = atoi(args[2]);
			//ProcessTimer = atoi(args[3]);

			scheduling_policy = 1;
			//DoContext = 1;
			if( Actuator1Start == 1){	

				resume(pid_actutator[0]);
				printf("this came!");			
				resume(pid_sensor[1]);
			}
			if(Actuator2Start == 1){
				resume(pid_sensor[0]);
				resume(pid_sensor[2]);
				resume(pid_actutator[1]);
			}
			printf("it ended");
			return 0;
		}


		fprintf(stderr, "%s: wrong Arguments\n", args[0]);
		fprintf(stderr, "Try '%s --help' for information\n",
				args[0]);
		
		return 1;

	}
	else{
		fprintf(stderr, "%s: too many arguments\n", args[0]);
		fprintf(stderr, "Try '%s --help' for more information\n",
				args[0]);
		return 1;
	}
	printf("starting the process here : \n");
	

	return 0;
}
