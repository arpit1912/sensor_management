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

		printf("Welcome to Sensor and Actuator Deployment system (SAD) developed on top of Xinu. The system supports 3 sensors (S1, S2, S3) and two actuators (A1 and A2). The system layout is as follows:\n");
		printf("1.  Actuator 1(A1) needs input from Sensor 2 (S2) to act\n");
		printf("2. Actuator 2 (A2) needs input from S1 and S2 to act\n");
		printf("Each actuator has two states, ON (1) and OFF (0). The sensors are linked to the respective actuators and switch ON/OFF along with them.\n");
		printf("\n");
		printf("To start SAD, enter the following command in Xinu shell:\n");
		printf("`start x y z`\n");
		printf("where:\n");
		printf("x: 0/1 (parameter to indicate whether actuator 1 is active)\n");
		printf("y: 0/1 (parameter to indicate whether actuator 1 is active)\n");
		printf("z: integer parameter to indicate time to run the system from\n");
		printf("Each sensor and actuator has its own process, and system implements lottery scheduling to switch between them. The sensors use buffers to transfer data to actuators. Actuator can only work if all the sensors it depends on have read and passed data onto it\n");
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
		fprintf(stderr, "%s: incorrect use of Arguments\n", args[0]);
		fprintf(stderr, "Try '%s --help' for more information\n",
				args[0]);
		return 1;
	}
	printf("starting the process here : \n");
	

	return 0;
}
