/*  main.c  - main */

#include <xinu.h>

void procA();
void procB();
void procC();
void procD();
void procE();

int32 i=0;
sid32 a,b,c,a1,b1,c1;

int32 v1=180,v2=20,v3=20;

struct data
{

uint32 first;

};


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


	pid_sensor[0]=create(procA, 1024, 40, "sensor_1",2,a,a1);

	resume(pid_sensor[0]);

	pid_sensor[1]=create(procB, 1024, 30, "sensor_2",2,b,b1);
	
	//resume(pid_sensor[1]);
	
	pid_sensor[2]=create(procC, 1024, 20, "sensor_3",2,c,c1);

	resume(pid_sensor[2]);

	pid_actutator[0]=create(procD, 1024, 15, "actuator_1",1,b1);
	resume(pid_actutator[0]);

	pid_actutator[1]=create(procE, 1024, 15, "actuator_2",2,a1,c1);	
	//resume(pid_actutator[0]);

	/* Wait for shell to exit and recreate it */


	total_tickets=60;	/* total_tickets */
	scheduling_policy = 1;

	/*
	while (TRUE) {
		receive();
		sleepms(200);
		kprintf("\n\nMain process recreating shell\n\n");
		resume(create(shell, 4096, 20, "shell", 1, CONSOLE));
	}
	*/

	return OK;
    
}


procA(
	sid32 a,
	sid32 a1	
)
{
//int32 i=0;
uint32 num;
bpid32 pool;
pool = mkbufpool(4, 4);
char* adr[4];
int order[4],count=0,in=0;

while(1)
{

if(i%5==0)
{

num = rand();
num = num % 40;

wait(a);
v1 = v1 + num;
signal(a);
signal(a1);

kprintf("\nSensor 1, Temperature Reading : %d\n\n",v1);


/*
struct data *ptr;

kprintf("\n Debug 1 \n");
if( count < 4 )
	{
		// count is a counter for no of buffer allocated till now. with getbuf() we get buffer address.
		adr[in] = getbuf(pool);
		count++;
		// we assign buffer address to struct data pointer so that the values we assign to struct are stored at our allocated buffer in memory.
		ptr = (struct data*)adr[in];
		
		order[in] = -1;
		
		int k=0;
		while(k<count)
		{
			order[k]++;
			k++;
		}
	in++;	
	kprintf("\n Debug 1 %d\n",in);
	}
	else
	{
	// If buffer is full we find the oldest allocated buffer and free it to reallocate.
		int oldest = 0;
		int k=0;
		while(k<count)
		{
			if(order[k]>order[oldest])
			{
				oldest = k;
			}
			k++;
		}
		
		freebuf(adr[oldest]);
		order[oldest] = -1;
		kprintf("\n No free Buffer available. Buffer %d vacated based on LRU policy. \n",oldest+1);
		
		k=0;
		while(k<count)
		{
			order[k]++;
			k++;
		}		
		adr[oldest] = getbuf(pool);
		ptr = (struct data*)adr[oldest];
	}

	// assigning i+1 to struct data which stores them at our allocated buffer location.
	kprintf("\n Debug 1 %d \n",num);	
	ptr->first = num;
	int j=0;

	kprintf("\n Debug 1 %d \n",ptr->first);
	while(j<count)
	{
	// printing  contents of all the allocated buffers.

	struct data *ptr1;

	ptr1 = (struct data*)adr[j];

	kprintf("\n Buffer %d contents, value : %d \n",j+1,ptr1->first);

	j++;
	}
*/


}

v1=180;
i++;
}

}


procB(
	sid32 b,
	sid32 b1
)
{
//int32 i=0;
int32 num;

while(1)
{

if(i%10==0)
{

num = rand();
num = num % 30;

wait(b);
v2 = v2 + num;
signal(b);
signal(b1);

kprintf("\nSensor 2, Soil Moisture Reading : %d\n\n",v2);

}

v2=20;
i++;
}

}


procC(
	sid32 c,
	sid32 c1
)
{
//int32 i=0;
int32 num;

while(1)
{

if(i%15==0)
{

num = rand();
num = num % 40;

wait(c);
v3 = v3 + num;
signal(c);
signal(c1);

kprintf("\nSensor 3, Humidity Reading : %d\n\n",v3);

}

v3=20;
i++;
}

}


procD(
	sid32 b1
)
{

while(1)
{
wait(b1);
kprintf("\n Actuator 1 \n\n");
}

}

procE(
	sid32 a1,
	sid32 c1
)
{

while(1)
{
wait(a1);
kprintf("\n Actuator 2 \n\n");
}

}

