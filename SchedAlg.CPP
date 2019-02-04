/********************************/
/*   Aryeh Nigri	338017866	*/
/*   Zeev Noiman	937574		*/
/********************************/

/********************************************************************/
/* A Small Real Time System for the Real-Time laboratory                 */
/* built by: A.Teitelbaum on an idea of H.G.Mendelbaum                  */
/* Jerusalem College of Technology, 5759-64 (1999)                            */
/* SchedAlgo64.CPP, short-term scheduler algorithms                */
/****************************************************************/

#include "SMARTS77.H"

int roundRobin()
// Round Robin short-term algorithm
{
	int count;
	int nextTask = (SMARTS.getCurrentTask() + 1) % SMARTS.getTotalTasks();
	for (count = 0;
		 SMARTS.getStatus(nextTask) != READY && count < SMARTS.getTotalTasks();
		 count++)
		nextTask = ++nextTask % SMARTS.getTotalTasks();
	if (count == SMARTS.getTotalTasks()) // not found READY task to run
		nextTask = SMARTS.getTotalTasks();
	return nextTask;
}


int EDF()
{
	int nextTask = SMARTS.getTotalTasks();	// the IDLE task
	int minPeriod = SMARTS.getCurrentPriority(nextTask);	// MAXINT is the IDLE's period
	// int minPeriod = MAXINT;
	int currentPeriod;	

	for (int count = 0; count < SMARTS.getTotalTasks(); count++)
	{
		if (SMARTS.getStatus(count) == READY)
		{
			currentPeriod = SMARTS.getCurrentPriority(count);
			
			if (currentPeriod < minPeriod) 
			{
				minPeriod = currentPeriod;
				nextTask = count;
			}
		}
	}

	return nextTask;
}


int RMS()
{
	int nextTask = SMARTS.getTotalTasks();	// the IDLE task
	int minPeriod = SMARTS.getPriority(nextTask);	// MAXINT is the IDLE's period
	// int minPeriod = MAXINT;	
	int period;

	for (int count = 0; count < SMARTS.getTotalTasks(); count++)
	{
		if (SMARTS.getStatus(count) == READY)
		{
			period = SMARTS.getPriority(count);
			
			if (period < minPeriod) 
			{
				minPeriod = period;
				nextTask = count;
			}
		}
	}

	return nextTask;
}