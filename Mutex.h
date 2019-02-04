/********************************/
/*   Aryeh Nigri	338017866	*/
/*   Zeev Noiman	937574		*/
/********************************/

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
    int originalPriority;
    int priorityDidChanged;
    char* name;
    PriorityQueue *waitingList;

  public:
    Mutex(int inheritance, char* name);
    ~Mutex();
    void acquire();
    void release();
};

Mutex::Mutex(int inheritance = 0, char* name = "default")
{
    s = 1;
    owner = -1;
    level = 0;
    priorityInheritanceFlag = inheritance;
    // originalPriority = 0;
    priorityDidChanged = 0;
    this->name = name;
    waitingList = new PriorityQueue();
}

Mutex::~Mutex()
{
    delete waitingList;
    delete name;
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
        fprintf(myOutput, "\n*** %s: ACQUIRED by Task %c ***\n", name, SMARTS.getName(currentTask));
        s = 0;
    }
    else
    {
        fprintf(myOutput, "\n*** %s: ACQUIRED by Task %c, but OWNER is Task %c ***\n", name, SMARTS.getName(currentTask), SMARTS.getName(owner));
        waitingList->push(currentTask);
        if (priorityInheritanceFlag)
        {
            int priority = SMARTS.getPriority(currentTask);
            int ownerPriority = SMARTS.getPriority(owner);
            if (priority < ownerPriority)
            {
                priorityDidChanged = 1;
                originalPriority = ownerPriority;
                fprintf(myOutput, "\n*** %s: Task %c INHERITING priority from Task %c ***\n", name, SMARTS.getName(owner), SMARTS.getName(currentTask));
                SMARTS.setPriority(owner, priority);
            }
        }
        fprintf(myOutput, "\n*** %s: SUSPENDING Task %c ***\n", name, SMARTS.getName(currentTask));
        SMARTS.suspended();
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
            fprintf(myOutput, "\n*** %s: Task %c DECREASING LEVEL to %i ***\n", name, SMARTS.getName(currentTask), level);            
            return;
        }
        else
        {
            // resets the priority when it was inherited
            if(priorityDidChanged){
                SMARTS.setPriority(owner, originalPriority);
                priorityDidChanged = 0;
            }

            fprintf(myOutput, "\n*** %s: RELEASED by Task %c ***\n", name, SMARTS.getName(owner));

            owner = -1;
            
            if(!waitingList->empty())// if (numberSuspended > 0)
            {
                int taskNum = waitingList->pop();
                fprintf(myOutput, "\n*** %s: RESUMING Task %c ***\n", name, SMARTS.getName(taskNum));
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