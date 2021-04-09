/*  main.c  - main */

#include <xinu.h>

void sensorA(sid32,sid32), sensorB(sid32,sid32), sensorC(sid32,sid32);
void actuatorA(sid32), actuatorB(sid32);

// Sensor's Buff Size

#define SensorABuffSize 4
#define SensorBBuffSize 2
#define SensorCBuffSize 1


// for global clock use clktime
// OccupiedBuffs array will store the allocated Buff At Each point

sid32 a,b,c,a1,b1,c1;

int32 v1=180,v2=60,v3=40;

struct data{
	uint32 first;
};


struct data* CreateBuff(bpid32 id,int32 *buffCount,int MaxSize,char* BuffAddr[], int* Priority);

#define SizePerBuff sizeof(struct data)

process	main(void)
{
	/* Run the Xinu shell */

	recvclr();
	a = semcreate(1);
	b = semcreate(1);
	c = semcreate(1);
	a1 = semcreate(0);
	b1 = semcreate(0);
	c1 = semcreate(0);
	
	pid_sensor[0]=create(sensorA, 1024, 40, "sensor_1",2,a,a1);


	pid_sensor[1]=create(sensorB, 1024, 30, "sensor_2",2,b,b1);

	//resume(pid_sensor[1]);

	pid_sensor[2]=create(sensorC, 1024, 20, "sensor_3",2,c,c1);


	// resume(pid_sensor[2]);

	pid_actutator[0]=create(actuatorA, 1024, 15, "actuator_1",1,b1);
	// resume(pid_actutator[0]);

	pid_actutator[1]=create(actuatorB, 1024, 15, "actuator_2",2,a1,c1);
	/* Wait for shell to exit and recreate it */

	resume(create(shell, 8192, 50, "shell", 1, CONSOLE));

	while (TRUE) {
		receive();
		sleepms(200);
		kprintf("\n\nMain process recreating shell\n\n");
		resume(create(shell, 4096, 20, "shell", 1, CONSOLE));
	}
	return OK;
    
}

void sensorA(sid32 a, sid32 b){

	printf("Size of each buff is: %d\n",SizePerBuff);
	PoolId[0] = mkbufpool(SizePerBuff,SensorABuffSize);
	kprintf("The Pool id is %d\n",PoolId[0]);
	OccupiedBuffs[0] = 0;
	int i = 0;	
	while(i<10){


			printf("Trial number %d\n\n", i);

			struct data *p = CreateBuff(PoolId[0], &OccupiedBuffs[0],SensorABuffSize,ABuffAddr,APriority);
			printf(" Occupied count is : %d \n ",OccupiedBuffs[0]);

			int temp = 0;

			while(temp < SensorABuffSize){
				printf(" Entry %d has address %s and %d priority \n",temp, ABuffAddr[temp], APriority[temp]);

				temp++;
			}

			i++;

	}
	printf("Process A Completed");


}


void sensorB(sid32 a, sid32 b){
	printf("Process B");
}


void sensorC(sid32 a, sid32 b){
	printf("Process C");
}


void actuatorA(sid32 a){
	printf("Actuactor A");
}


void actuatorB(sid32 a){
	printf("Actuactor B");
}

struct data* CreateBuff(bpid32 id,int32 *buffCount,int MaxSize,char* BuffAddr[], int* Priority){	// This function take care of the LRU policy while Allocation and Deallocation in Buffer
	
	if(*buffCount < MaxSize){
		printf("working till here! %d \n", *buffCount);
		char* pt = getbuf(id);
		BuffAddr[*buffCount] = pt;
		Priority[*buffCount] = -1;
		int i = 0;	
		while(i <= *buffCount){
			Priority[i++]++;	// Increasing the Priority Number higher number will get removed first
		}
		printf("working after assigning \n");
		return (struct data*)BuffAddr[(*buffCount)++];	// Returning the Buffer Addr 
	}
	else{
		int i = 0;
		int max_index = 0;	
		
		while(i<MaxSize){
			
			if(Priority[i]> Priority[max_index]){
				max_index = i;	// finding the LRU buffer The max priority buffer will be the least recent one
			}
			i++;
		}		
		
		printf("working on fully filled! %d  %d \n", *buffCount, max_index);
		printf("The free address is : %d \n", BuffAddr[max_index]);
		freebuf(BuffAddr[max_index]);	// making the buffer free

		printf("working on fully filled! %d  %d \n", *buffCount, max_index);
		Priority[max_index] = -1;


		printf("working on fully filled! %d  %d \n", *buffCount, max_index);

		BuffAddr[max_index] = getbuf(id);	// allocating the Buffer again
		kprintf("\nnew buffer is allocated at %d \n",max_index);
		i = 0;	
		while(i < *buffCount){
			Priority[i++]++;	// Increasing the Priority Number
		}
		return (struct data*)BuffAddr[max_index];	// Returning the Buffer Addr

	}
}