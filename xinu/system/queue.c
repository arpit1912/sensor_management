/* queue.c - enqueue, dequeue */

#include <xinu.h>

struct qentry	queuetab[NQENT];	/* Table of process queues	*/

/*------------------------------------------------------------------------
 *  enqueue  -  Insert a process at the tail of a queue
 *------------------------------------------------------------------------
 */
pid32	enqueue(
	  pid32		pid,		/* ID of process to insert	*/
	  qid16		q		/* ID of queue to use		*/
	)
{
	qid16	tail, prev;		/* Tail & previous node indexes	*/

	if (isbadqid(q) || isbadpid(pid)) {
		return SYSERR;
	}

	tail = queuetail(q);
	prev = queuetab[tail].qprev;

	queuetab[pid].qnext  = tail;	/* Insert just before tail node	*/
	queuetab[pid].qprev  = prev;
	queuetab[prev].qnext = pid;
	queuetab[tail].qprev = pid;
	return pid;
}

/*------------------------------------------------------------------------
 *  dequeue  -  Remove and return the first process on a list
 *------------------------------------------------------------------------
 */
pid32	dequeue(
	  qid16		q		/* ID of queue to use		*/
	)
{
	pid32	pid;			/* ID of process removed	*/

	if (isbadqid(q)) {
		return SYSERR;
	} else if (isempty(q)) {
		return EMPTY;
	}

	pid = getfirst(q);
	queuetab[pid].qprev = EMPTY;
	queuetab[pid].qnext = EMPTY;
	return pid;
}

/*------------------------------------------------------------------------
 *  dequeue  -  Remove and return the process from list
 *------------------------------------------------------------------------
 */
pid32	dequeue_process(
	  qid16		q,		/* ID of queue to use		*/
	  int32		num
	)
{
	pid32	pid;			/* ID of process removed	*/

	if (isbadqid(q)) {
		return SYSERR;
	} else if (isempty(q)) {
		return EMPTY;
	}

	int16	curr;			/* Runs through items in a queue*/
	int16	prev;			/* Holds previous node index	*/
	int16	next;			/* holds next node */

	curr = firstid(q);
	while(1)
	{
		if(num < queuetab[curr].qkey)
			{
				pid = curr;
				break;
			}
		else
			{
				num = num - queuetab[curr].qkey;
				curr = queuetab[curr].qnext;		
			}

	}

	prev = queuetab[curr].qprev;
	next = queuetab[curr].qnext;
	queuetab[prev].qnext =	next;
	queuetab[next].qprev = prev;
	return pid;
}