/*  main.c  - main */

#include <xinu.h>

// Function Declaration for processes and actuators 
void sensorA(sid32,sid32), sensorB(sid32,sid32), sensorC(sid32,sid32);
void actuatorA(sid32), actuatorB(sid32,sid32);

int buffCount = 0;	// for keeping track of filled buffers

// Semaphore for Process Synchornization 
sid32 a,b,c,a1,b1,c1;

// Base value for reading 
int32 v1=180,v2=60,v3=40;

// Struct for the purpose of data storing in Memory Buffers. 
struct data{
	uint32 first;
};

// Function Declaration 
struct data* CreateBuff(bpid32 id,int32 *buffCount,int MaxSize,char* ABuffAddr[], int* APriority);
struct data* GetBuff(int32 *buffCount,int MaxSize,char* ABuffAddr[], int* APriority);

#define SizePerBuff sizeof(struct data)

process	main(void)
{
	/* Run the Xinu shell */

	recvclr();
	resume(create(shell, 8192, 50, "shell", 1, CONSOLE));

	// Initialize the semaphores 
	
	a = semcreate(1);
	b = semcreate(1);
	c = semcreate(1);
	a1 = semcreate(0);
	b1 = semcreate(0);
	c1 = semcreate(0);
	
	// Create all the process and store their process ID's
	
	pid_sensor[0]=create(sensorA, 1024, 40, "sensor_1",2,a,a1);

	pid_sensor[1]=create(sensorB, 1024, 30, "sensor_2",2,b,b1);

	pid_sensor[2]=create(sensorC, 1024, 20, "sensor_3",2,c,c1);

	pid_actutator[0]=create(actuatorA, 1024, 15, "actuator_1",1,b1);

	pid_actutator[1]=create(actuatorB, 1024, 15, "actuator_2",2,a1,c1);


	while (TRUE) {
		receive();
		sleepms(200);
		kprintf("\n\nMain process recreating shell:> \n\n");

		resume(create(shell, 4096, 20, "shell", 1, CONSOLE));
	}
	return OK;
    
}

// Sensor A process function 
void sensorA(sid32 a, sid32 b){

	DoContext = 1;
	// Store the pool Id for buffer 
	PoolId[0] = mkbufpool(SizePerBuff + 4,SensorABuffSize);
	// Occupied buffer for poolid[0]
	OccupiedBuffs[0] = 0;
	// Offset for random number generation. 
	int32 offset = 20;
	while(1){

			// Sensor runs if clock time is divisible by 3 
			if(clktime % 3 == 0){
					// get the address of buffer location
					struct data *p = CreateBuff(PoolId[0], &OccupiedBuffs[0],SensorABuffSize,ABuffAddr,APriority);

					// getlock on buffer location 
					wait(a);
					p->first = v1 + rand()%offset;
					kprintf("Sensor A scheduled, writes it's data %d into buffer \n", p->first);
					// releaselock on buffer location 
					signal(a);
					// Signal semaphore that data written in memory
					if( semtab[b].scount <= 0){
						shreyansh(b);
					}
					resched();
			}

			
			
	}
}

// Sensor B process function 
void sensorB(sid32 a, sid32 b){
	DoContext = 1;
	// Store the pool Id for buffer 
	PoolId[1] = mkbufpool(SizePerBuff + 4,SensorBBuffSize);
	// Occupied buffer for poolid[1]
	OccupiedBuffs[1] = 0;
	// Offset for random number generation. 
	int32 offset = 30;

	while(1){

			// Sensor runs if clock time is divisible by 5
			if(clktime % 5 == 0){
					// get the address of buffer location
					struct data *p = CreateBuff(PoolId[1], &OccupiedBuffs[1],SensorBBuffSize,BBuffAddr,BPriority);
					// getlock on buffer location
					wait(a);
					p->first = v2 + rand()%offset;
					kprintf("Sensor B scheduled, writes it's data %d into buffer \n", p->first);
					// releaselock on buffer location
					signal(a);
					// Signal semaphore that data written in memory
					if( semtab[b].scount <= 0){
						shreyansh(b);
					}

					resched();
			}
			
	}
}

// Sensor C process function 
void sensorC(sid32 a, sid32 b){
	
	DoContext = 1;
	// Store the pool Id for buffer
	PoolId[2] = mkbufpool(SizePerBuff + 4,SensorCBuffSize);
	// Occupied buffer for poolid[2]
	OccupiedBuffs[2] = 0;
	// Offset for random number generation. 
	int32 offset = 50;
	while(1){
			// Sensor runs if clock time is divisible by 7
			if(clktime % 7 == 0){
					// get the address of buffer location
					struct data *p = CreateBuff(PoolId[2], &OccupiedBuffs[2],SensorCBuffSize,CBuffAddr,CPriority);
					// getlock on buffer location
					wait(a);
					p->first = v3 + rand()%offset;
					kprintf("Sensor C scheduled, writes it's data %d into buffer \n", p->first);
					// releaselock on buffer location
					signal(a);
					// Signal semaphore that data written in memory
					if( semtab[b].scount <= 0){
						shreyansh(b);
					}

					resched();
			}
			
	}
}


// Actuator A process function 
void actuatorA(sid32 a){
	DoContext = 1;
	while(1){
		// Check for wait condition on semaphore a
		dinkar(a,"Actuator A scheduled, no data found from sensor B so A sent to WAIT state");
		printf("Actuator A scheduled, reads sensor data from buffer and acts on it\n");
		struct data *p = GetBuff(&OccupiedBuffs[1],SensorBBuffSize,BBuffAddr,BPriority);
		resched();
	}
	
}


// Actuator B process function 
void actuatorB(sid32 a,sid32 b){
	DoContext = 1;
	while(1){	
		// Check for wait condition on semaphore a,b 	
		dinkar(a,"Actuator B scheduled, no data found from sensor A so B sent to WAIT state");
		dinkar(b,"Actuator B scheduled, no data found from sensor C so B sent to WAIT state");
		printf("Actuator B scheduled, reads sensor data from buffer and acts on it\n");
		struct data *p = GetBuff(&OccupiedBuffs[0],SensorABuffSize,ABuffAddr,APriority);
		struct data *q = GetBuff(&OccupiedBuffs[2],SensorCBuffSize,CBuffAddr,CPriority);
		resched();
	}	
}


struct data* CreateBuff(bpid32 id,int32 *buffCount,int MaxSize,char* ABuffAddr[], int* APriority){	
// This function take care of the LRU policy while Allocation and Deallocation in Buffer
	
	if(*buffCount < MaxSize){
		char* pt = getbuf(id);
		ABuffAddr[*buffCount] = pt;
		APriority[*buffCount] = -1;
		int i = 0;	
		while(i <= *buffCount){
			APriority[i++]++;// Increasing the APriority Number higher number will get removed first
		}
		return (struct data*)ABuffAddr[(*buffCount)++];	// Returning the Buffer Addr 
	}
	else{
		int i = 0;
		int max_index = 0;	
		
		while(i<MaxSize){
			
			if(APriority[i]> APriority[max_index]){
				max_index = i;	
				// finding the LRU buffer The max priority buffer will be the least recent one
			}
			i++;
		}		

		freebuf(ABuffAddr[max_index]);			// making the buffer free
		APriority[max_index] = -1;

		ABuffAddr[max_index] = getbuf(id);		// allocating the Buffer again
		//kprintf("\nnew buffer is allocated at %d \n",max_index);
		i = 0;	
		while(i < *buffCount){
			APriority[i++]++;			// Increasing the APriority Number
		}
		return (struct data*)ABuffAddr[max_index];	// Returning the Buffer Addr

	}
}

// getting the buffer for data storing 
struct data* GetBuff(int32 *buffCount,int MaxSize,char* ABuffAddr[], int* APriority)
{
	int max_index = 0;
	int i = 0;

	if(*buffCount < MaxSize){	
		while(i < *buffCount){
			
			if(APriority[i] > APriority[max_index]){
				max_index = i;
			}
			i++;
		}
		
	}
	else{
		while(i < MaxSize){
			
			if(APriority[i] > APriority[max_index]){
				max_index = i;	
			}
			i++;
		}		
		
	}
	return (struct data*)ABuffAddr[max_index];

}
