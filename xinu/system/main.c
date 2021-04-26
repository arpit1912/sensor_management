/*  main.c  - main */

#include <xinu.h>

void sensorA(sid32,sid32), sensorB(sid32,sid32), sensorC(sid32,sid32);
void actuatorA(sid32), actuatorB(sid32,sid32);

// char* ABuffAddr[4];	// For keeping track of the Buffers
// int APriority[4];	// For storing the APriority of the Buffers
int buffCount = 0;	// for keeping track of filled buffers


sid32 a,b,c,a1,b1,c1;

sid32 d1;
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
	
	//resume(pid_sensor[1]);
	// scheduling_policy = 1;
	total_tickets = 55;

	

	while (TRUE) {
		receive();
		sleepms(200);
		kprintf("\n\nMain process recreating shell:> \n\n");

		resume(create(shell, 4096, 20, "shell", 1, CONSOLE));
	}
	return OK;
    
}

void sensorA(sid32 a, sid32 b){

	DoContext = 1;
	PoolId[0] = mkbufpool(SizePerBuff + 4,SensorABuffSize);
	OccupiedBuffs[0] = 0;
	int32 start = 70;
	int32 offset = 20;
	while(1){
if(scheduling_policy == 0)
				return;
			
			if(clktime % 3 == 0){
					kprintf("sensor A started\n");
					struct data *p = CreateBuff(PoolId[0], &OccupiedBuffs[0],SensorABuffSize,ABuffAddr,APriority);

					wait(a);
					p->first = 70 + rand()%offset;
					signal(a);
					if( semtab[b].scount <= 0){
					//	kprintf("Semaphore value 1: %d\n",semtab[b].scount);
						shreyansh(b);
					//	kprintf("Semaphore value 2: %d\n",semtab[b].scount);
					}
					kprintf("sensor A Ended\n");
					resched();
			}

			
			
	}
	//printf("Process A Completed");
}


void sensorB(sid32 a, sid32 b){
	DoContext = 1;
	PoolId[1] = mkbufpool(SizePerBuff + 4,SensorBBuffSize);
	OccupiedBuffs[1] = 0;
	int32 start = 80;
	int32 offset = 30;
	int t = 500;
	while(1){
			if(scheduling_policy == 0)
				return;
			
			if(clktime % 5 == 0){

					kprintf("sensor B started\n");
					//kprintf("New Entry coming at %d\n",clktime);
					struct data *p = CreateBuff(PoolId[1], &OccupiedBuffs[1],SensorBBuffSize,BBuffAddr,BPriority);
					
					wait(a);
					p->first = 70 + rand()%offset;
					//kprintf(" giving output %d \n", p->first);
					signal(a);
					if( semtab[b].scount <= 0){
						//kprintf("Semaphore value 1: %d\n",semtab[b].scount);
						shreyansh(b);
						//kprintf("Semaphore value 2: %d\n",semtab[b].scount);
					}

					kprintf("sensor B ended\n");
					resched();
			}
			
	}
}


void sensorC(sid32 a, sid32 b){
	
	DoContext = 1;
	PoolId[2] = mkbufpool(SizePerBuff + 4,SensorCBuffSize);
	OccupiedBuffs[2] = 0;
	int32 start = 20;
	int32 offset = 50;
	while(1){
if(scheduling_policy == 0)
				return;
			
			if(clktime % 7 == 0){

					kprintf("sensor C started\n");
					//kprintf("New Entry coming at %d\n",clktime);
					struct data *p = CreateBuff(PoolId[2], &OccupiedBuffs[2],SensorCBuffSize,CBuffAddr,CPriority);
					
					wait(a);
					p->first = 70 + rand()%offset;
					signal(a);
					if( semtab[b].scount <= 0){
					//	kprintf("Semaphore value 1: %d\n",semtab[b].scount);
						shreyansh(b);
					//	kprintf("Semaphore value 2: %d\n",semtab[b].scount);
					}

					kprintf("sensor C ended\n");
					resched();
			}
			
	}
	printf("Process c Completed");
}


void actuatorA(sid32 a){
	DoContext = 1;
	while(1){
if(scheduling_policy == 0)
				return;
			
					kprintf("actuator A started\n");
	kprintf("Semaphore value 3: %d\n",semtab[a].scount);
	dinkar(a);
	kprintf("Semaphore value 4: %d\n",semtab[a].scount);
	printf("Actuactor A");

					kprintf("actuator A ended\n");
	resched();
	}
	
}


void actuatorB(sid32 a,sid32 b){
	DoContext = 1;
	while(1){
if(scheduling_policy == 0)
				return;
			
					kprintf("actuator B started\n");
	kprintf("Semaphore value 3: %d\n",semtab[a].scount);
	dinkar(a);
	dinkar(b);
	kprintf("Semaphore value 4: %d\n",semtab[a].scount);
	printf("Actuactor B");

					kprintf("actuator B ended\n");
	resched();
	}	
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
		//kprintf("\nnew buffer is allocated at %d \n",max_index);
		i = 0;	
		while(i < *buffCount){
			APriority[i++]++;	// Increasing the APriority Number
		}
		return (struct data*)ABuffAddr[max_index];	// Returning the Buffer Addr

	}
}