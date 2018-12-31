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
    int numberSuspended;
    PriorityQueue *waitingList;

  public:
    Mutex();
    ~Mutex();
    void acquire();
    void release();
};

Mutex::Mutex()
{
    s = 1;
    owner = -1;
    level = 0;
    numberSuspended = 0;
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
        s = 0;
    }
    else
    {
        numberSuspended++;
        waitingList->push(currentTask);
        // SMARTS.suspend(currentTask);
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
            return;
        }
        else
        {
            owner = -1;
            if (numberSuspended > 0)
            {
                numberSuspended--;
                // int taskNum = waitingList->pop();
                // SMARTS.resume(taskNum);
                waitingList->pop();
                SMARTS.resume(currentTask);
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