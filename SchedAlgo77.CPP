/********************************************************************/
/* A Small Real Time System for the Real-Time laboratory                 */
/* built by: A.Teitelbaum on an idea of H.G.Mendelbaum                  */
/* Jerusalem College of Technology, 5759-64 (1999)                            */
/* SchedAlgo64.CPP, short-term scheduler algorithms                */
/****************************************************************/

#include "smarts77.h"

int roundRobin()
// Round Robin short-term algorithm
{
	int count;
	int nextTask = (SMARTS.getCurrentTask() + 1) % SMARTS.getTotalTasks();
	for (count = 0;
		 SMARTS.getStatus(nextTask) != READY && count < SMARTS.getTotalTasks();
		 count++)
		nextTask = ++nextTask % SMARTS.getTotalTasks();
	if (count == SMARTS.getTotalTasks()) // no found READY task to run
		nextTask = SMARTS.getTotalTasks();
	return nextTask;
}

int EDF()
{
	int count, nextTask, currentPeriod;
	int currentTask = SMARTS.getCurrentTask();
	int minPeriod = SMARTS.getCurrentPriority(currentTask);

	for (count = 0; count < SMARTS.getTotalTasks(); count++)
	{
		currentTask = (currentTask + 1) % SMARTS.getTotalTasks();
		currentPeriod = SMARTS.getCurrentPriority(currentTask);

		if (SMARTS.getStatus(currentTask) == READY && currentPeriod < minPeriod)
		{
			minPeriod = currentPeriod;
			nextTask = currentTask;
		}
	}

	return nextTask;
}
