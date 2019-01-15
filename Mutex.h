#ifndef MUTEX_H
#define MUTEX_H

#include "smarts77.h"
#include "PQueue.h"

Parallelism SMARTS;

// This class will activate a synchronization mechanism of the Mutex binary semaphore
// (the semaphore algorithm is described in the "Chapter 4" presentation).
// The Acquire and Release methods should be atomic.
class Mutex
{
  private:
    int s;
    int owner;
    int level;
    int priorityInheritanceFlag;
    PriorityQueue *waitingList;

  public:
    Mutex(int inheritance);
    ~Mutex();
    void acquire();
    void release();
};

Mutex::Mutex(int inheritance = 0)
{
    s = 1;
    owner = -1;
    level = 0;
    priorityInheritanceFlag = inheritance;
    waitingList = new PriorityQueue();
}

Mutex::~Mutex()
{
    delete waitingList;
}

// Acquire:
//     if (s == 1 || Owner == CurrentTask)
//         s = 0;
//     else
//     {
//         numberSuspended++;
//         waitingList.Put(currentTask);
//         Suspend(currentTask);
//     }
//     Owner = CurrentTask;
//     Level++;
void Mutex::acquire()
{
    SMARTS.contextSwitchOff();

    int currentTask = SMARTS.getCurrentTask();
    if (s == 1 || owner == currentTask)
    {
        fprintf(myOutput, "\n*** Task %c ACQUIRING mutex ***\n", SMARTS.getName(currentTask));
        s = 0;
    }
    else
    {
        fprintf(myOutput, "\n*** Task %c acquired busy mutex, SUSPENDING IT ***\n", SMARTS.getName(currentTask));
        waitingList->push(currentTask);
        if (priorityInheritanceFlag)
        {
            int priority = SMARTS.getPriority(currentTask);
            if (priority < SMARTS.getPriority(owner))
            {
                SMARTS.setPriority(owner, priority);
                fprintf(myOutput, "\n*** Task %c INHERITING priority from Task %c ***\n", SMARTS.getName(owner), SMARTS.getName(currentTask));
            }
        }
        SMARTS.suspended();
        // fprintf(myOutput, "*** Task %c acquired busy mutex, SUSPENDING IT ***", SMARTS.getName(currentTask));
    }
    owner = currentTask;
    level++;

    SMARTS.contextSwitchOn();
}

// Release:
//     if (Owner == CurrentTask)
// 		if (--level)
// 			return;
// 		else
//         {
// 			Owner = -1;
//             if (numberSuspended > 0)
//             {
//                 numberSuspended--;
//                 waitingList.Get(currentTask);
//                 Resume(currentTask);
//             }
//             else
//                 s = 1;
// 	    }
void Mutex::release()
{
    SMARTS.contextSwitchOff();

    int currentTask = SMARTS.getCurrentTask();
    if (owner == currentTask)
    {
        if (--level)
        {
            fprintf(myOutput, "\n*** Task %c DECREASING LEVEL to %i ***\n", SMARTS.getName(currentTask), level);            
            return;
        }
        else
        {
            owner = -1;
            fprintf(myOutput, "\n*** Task %c RELEASING mutex ***\n", SMARTS.getName(currentTask));
            if(!waitingList->empty())// if (numberSuspended > 0)
            {
                int taskNum = waitingList->pop();
                fprintf(myOutput, "\n*** RESUMING Task %c ***\n", SMARTS.getName(taskNum));
                SMARTS.resume(taskNum);
            }
            else
            {
                s = 1;
            }
        }
    }

    SMARTS.contextSwitchOn();
}

#endif // MUTEX_H