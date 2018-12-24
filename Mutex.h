// This class will activate a synchronization mechanism of the Mutex binary semaphore 
// (the semaphore algorithm is described in the "Chapter 4" presentation).

// The Acquire and Release methods should be atomic.

class Mutex
{
    // nao lembro qq tem num mutex
    int s;
    PriorityQueue q;
};


// Acquire and Release are Atomic methods

// Mutex Algorithm

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



// Binary Algorithm


// Acquire:
//     if (s == 1) 
//         s = 0; 
//     else 
//     {
//         numberSuspended++;
//         waitingList.Put(currentTask);
//         Suspend(currentTask);
//     }


// Release:
//     if (numberSuspended > 0) 
//     {
//         numberSuspended--;
//         waitingList.Get(currentTask);
//         Resume(currentTask);
//     }
//     else
//         s = 1;