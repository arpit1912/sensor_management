/*  main.c  - main */

#include <xinu.h>

void sensorA(sid32,sid32), sensorB(sid32,sid32), sensorC(sid32,sid32);
void actuatorA(sid32), actuatorB(sid32,sid32);

// char* ABuffAddr[4];	// For keeping track of the Buffers
// int APriority[4];	// For storing the APriority of the Buffers
int buffCount = 0;	// for keeping track of filled buffers


sid32 a,b,c,a1,b1,c1;

int32 v1=180,v2=60,v3=40;

struct data{
	uint32 first;
};


struct data* CreateBuff(bpid32 id,int32 *buffCount,int MaxSize,char* ABuffAddr[], int* APriority);
// struct data* CreateBuff(bpid32);

#define SizePerBuff sizeof(struct data)

process	main(void)
{
	/* Run the Xinu shell */

	recvclr();
	resume(create(shell, 8192, 50, "shell", 1, CONSOLE));

	a = semcreate(1);
	b = semcreate(1);
	c = semcreate(1);
	a1 = semcreate(0);
	b1 = semcreate(0);
	c1 = semcreate(0);
	
	pid_sensor[0]=create(sensorA, 1024, 40, "sensor_1",2,a,a1);

	pid_sensor[1]=create(sensorB, 1024, 30, "sensor_2",2,b,b1);

	pid_sensor[2]=create(sensorC, 1024, 20, "sensor_3",2,c,c1);

	pid_actutator[0]=create(actuatorA, 1024, 15, "actuator_1",1,b1);

	pid_actutator[1]=create(actuatorB, 1024, 15, "actuator_2",2,a1,c1);
	/* Wait for shell to exit and recreate it */

	resume(pid_sensor[0]);
	resume(pid_sensor[1]);
	resume(pid_sensor[2]);
	resume(pid_actutator[0]);
	resume(pid_actutator[1]);

	total_tickets = 120;
	scheduling_policy = 1;
	

	while (TRUE) {
		receive();
		sleepms(200);
		kprintf("\n\nMain process recreating shell\n\n");
		resume(create(shell, 4096, 20, "shell", 1, CONSOLE));
	}
	return OK;
    
}

void sensorA(sid32 a, sid32 b){

	PoolId[0] = mkbufpool(SizePerBuff + 4,SensorABuffSize);
	OccupiedBuffs[0] = 0;
	int32 start = 70;
	int32 offset = 20;
	while(1){

			if(clktime % 3 == 0){
					kprintf("New Entry coming at %d\n",clktime);
					struct data *p = CreateBuff(PoolId[0], &OccupiedBuffs[0],SensorABuffSize,ABuffAddr,APriority);

					wait(a);
					p->first = 70 + rand()%offset;
					signal(a);
					signal(b);

					int j = 0;
					while(j<OccupiedBuffs[0]){
						struct data* temp = (struct data*) ABuffAddr[j];
						kprintf("Buffer %d Values are: \n\tfirst: %d \n",j,temp->first);
						j++;	
					}
					sleepms(1000);
			}
	}
	printf("Process A Completed");
}


void sensorB(sid32 a, sid32 b){
	
	PoolId[1] = mkbufpool(SizePerBuff + 4,SensorBBuffSize);
	OccupiedBuffs[1] = 0;
	int32 start = 80;
	int32 offset = 30;
	while(1){

			if(clktime % 5 == 0){
					kprintf("New Entry coming at %d\n",clktime);
					struct data *p = CreateBuff(PoolId[1], &OccupiedBuffs[1],SensorBBuffSize,BBuffAddr,BPriority);
					
					wait(a);
					p->first = 70 + rand()%offset;
					signal(a);
					signal(b);

					int j = 0;
					while(j<OccupiedBuffs[1]){
						struct data* temp = (struct data*) BBuffAddr[j];
						kprintf("Buffer %d Values are: \n\tfirst: %d \n",j,temp->first);
						j++;	
					}
					sleepms(2000);
			}
	}
	printf("Process B Completed");
}


void sensorC(sid32 a, sid32 b){

	PoolId[2] = mkbufpool(SizePerBuff + 4,SensorCBuffSize);
	OccupiedBuffs[2] = 0;
	int32 start = 20;
	int32 offset = 50;
	while(1){

			if(clktime % 7 == 0){
					kprintf("New Entry coming at %d\n",clktime);
					struct data *p = CreateBuff(PoolId[2], &OccupiedBuffs[2],SensorCBuffSize,CBuffAddr,CPriority);
					
					wait(a);
					p->first = 70 + rand()%offset;
					signal(a);
					signal(b);

					int j = 0;
					while(j<OccupiedBuffs[2]){
						struct data* temp = (struct data*) CBuffAddr[j];
						kprintf("Buffer %d Values are: \n\tfirst: %d \n",j,temp->first);
						j++;	
					}
					sleepms(3000);
			}
	}
	printf("Process c Completed");
}


void actuatorA(sid32 a){
	wait(a);
	printf("Actuactor A");
}


void actuatorB(sid32 a,sid32 b){
	wait(a);
	wait(b);
	printf("Actuactor B");
}

struct data* CreateBuff(bpid32 id,int32 *buffCount,int MaxSize,char* ABuffAddr[], int* APriority){	// This function take care of the LRU policy while Allocation and Deallocation in Buffer
	
	if(*buffCount < MaxSize){
		char* pt = getbuf(id);
		ABuffAddr[*buffCount] = pt;
		APriority[*buffCount] = -1;
		int i = 0;	
		while(i <= *buffCount){
			APriority[i++]++;	// Increasing the APriority Number higher number will get removed first
		}
		return (struct data*)ABuffAddr[(*buffCount)++];	// Returning the Buffer Addr 
	}
	else{
		int i = 0;
		int max_index = 0;	
		
		while(i<MaxSize){
			
			if(APriority[i]> APriority[max_index]){
				max_index = i;	// finding the LRU buffer The max priority buffer will be the least recent one
			}
			i++;
		}		

		freebuf(ABuffAddr[max_index]);	// making the buffer free
		APriority[max_index] = -1;

		ABuffAddr[max_index] = getbuf(id);	// allocating the Buffer again
		kprintf("\nnew buffer is allocated at %d \n",max_index);
		i = 0;	
		while(i < *buffCount){
			APriority[i++]++;	// Increasing the APriority Number
		}
		return (struct data*)ABuffAddr[max_index];	// Returning the Buffer Addr

	}
}