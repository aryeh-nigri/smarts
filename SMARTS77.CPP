/****************************************************************/
/* A Small Real Time System for the Real-Time laboratory        */
/* built by: A.Teitelbaum on an idea of H.G.Mendelbaum          */
/* Jerusalem College of Technology, 5759-64 (1999)              */
/* update  Tishrey   5777                                       */
/* SMARTS77.CPP, SMARTS class body                              */
/****************************************************************/
#include "SMARTS77.H"
#include <stdlib.h>

/**********    Function     **********/
unsigned getTimerClocks()
// Gets the remaining clocks of the timer register
{
	unsigned clocks;
	/* latch counter #0 */
	outportb(0x43, 0x00);
	/* read counter #0 low byte */
	clocks = inportb(0x40);
	/* read counter #0 high byte */
	clocks += inportb(0x40) << 8;
	return clocks;
}
////////////////////////////////////////////////////
/**********    class body:  Parallelism     **********/
Parallelism::Parallelism()
{
	currentTask = 0;
	sleepTasks = 0;
	activeTasks = 0;
	totalTasks = 0;
	deadlock = false;
<<<<<<< HEAD
	// contextSwitchFlag = true;
=======
	//contextSwitchFlag = true;
>>>>>>> e00d69394674b08fc7dac2d6b63da4f6a12306b4
	endOfTimeSlice = true;
	error = false;
	allFinished = false;
}

void Parallelism::externalFunctions(void interrupt (*timerInterruptHandler)(...),
									void far *scheduler, void far *userTaskEnd,
									int far (*algorithm)())
// Sets the external functions
{
	this->timerInterruptHandler = timerInterruptHandler;
	this->scheduler = scheduler;
	this->userTaskEnd = userTaskEnd;
	this->algorithm = algorithm;
	
	contextSched.declare(scheduler, userTaskEnd, 'S', 0, 1); // prepare the stack of the scheduler task
	for (int i = MaxStack - 1; i >= (MaxStack - 14); i--)
		schedCopy[i] = contextSched.stack[i];
}

int Parallelism::declareTask(void far *code, char name, int period, int cycles)
// Insert a new task entry in SMARTS context array [ ]
{
	if (totalTasks < MaxTask - 1)
	{
		context[totalTasks++].declare(code, userTaskEnd, name, period, cycles);
		++activeTasks;
		return true;
	}
	else
		return false;
}

void Parallelism::runTheTasks()
// Start running all tasks declared in SMARTS.
{
	context[totalTasks].status = READY; //for task 'runTheTasks' (IDLE task)
	context[totalTasks].priority = MAXINT;
	context[totalTasks].currentPriority = MAXINT;
	context[totalTasks].name = 'I';
	context[totalTasks].currentCycles = 0;
	// context[totalTasks].contextSwitchFlag = 1;


	currentTask = totalTasks;

	asm cli;							 // forbids interrupts (clear interrupts) while changing the interrupt vect
										 // saves the original BIOS userInt in our variable 'userIntAddress' to be restored at the end
	userIntAddress = getvect(userInt);   // BIOS userInt 0x60  (unused by PC)
										 // puts the normal BIOS timerInt into the unused userInt address
	setvect(userInt, getvect(timerInt)); // timerInt 0x08

	// sets our SMARTS external function 'timerInterruptHandler' as the new PC hard interrupt time handler
	setvect(timerInt, timerInterruptHandler);
	asm sti; // allows back interrupts (set interrupts)

	// waits for end of runTheTasks (end of all the tasks)
	while (true)
	{
		if (deadlock)
		{
			textcolor(RED);
			cprintf("\n\n\rExit : deadlock");
			fprintf(myOutput, "\n\n\rExit : deadlock");
			break;
		}
		if(error == true){
			textcolor(RED);
			cprintf("\n\n\rExit : ERROR");
			fprintf(myOutput, "\n\n\rExit : ERROR");
			break;
		}
		if (allFinished == true)// && activeTasks == 0)	// PRECISA ser so allFinished
		{
			textcolor(BLUE);
			cprintf("\n\n\rExit : finish");
			fprintf(myOutput, "\n\n\rExit : finish");
			break;
		}
	}

	// restore the original BIOS 'interrupt vector' at the end before returning to regular DOS
	asm cli;							 // no interrupts
	setvect(timerInt, getvect(userInt)); // restore original BIOS time handler
	setvect(userInt, userIntAddress);	// restore original BIOS userInt
	asm sti;							 // yes interrupts
}

void Parallelism::callScheduler()
// Return the control to the scheduler, this sets ON the software interrupt ProgInt flag
{
	setProgInt();
	asm int timerInt;
}

void Parallelism::restoreSchedStack()
// Restore the scheduler stack
{
	for (int i = MaxStack - 1; i >= (MaxStack - 14); i--)
		contextSched.stack[i] = schedCopy[i];
}

int Parallelism::getCurrentTask()
{
	return currentTask;
}

void Parallelism::setCurrentTask(int taskNum)
// Sets the next task to be run
{
	if (taskNum <= totalTasks)
		currentTask = taskNum;
}

int Parallelism::getTotalTasks()
// Gets total tasks declared
{
	return totalTasks;
}

int Parallelism::getDeadlock()
{
	return deadlock;
}

void Parallelism::setDeadlock()
{
	deadlock = true;
}

int Parallelism::contextSwitchOn()
// flag which enables context switch
{
	if (endOfTimeSlice) //is current time slice finished ?
	{
		endOfTimeSlice = false;
<<<<<<< HEAD
		// contextSwitchFlag = true;
		context[currentTask].setContextSwitchOn();
		callScheduler(); // return control to the scheduler
		return 1;
	}
	// contextSwitchFlag = true;
	context[currentTask].setContextSwitchOn();
=======
		context[currentTask].setContextSwitchOn();
	 // contextSwitchFlag = true;
		callScheduler(); // return control to the scheduler
		return 1;
	}
	context[currentTask].setContextSwitchOn();
	//contextSwitchFlag = true;
>>>>>>> e00d69394674b08fc7dac2d6b63da4f6a12306b4
	return 0;
}

void Parallelism::contextSwitchOff()
// Disable context switch
{
<<<<<<< HEAD
	// contextSwitchFlag = false;
	context[currentTask].setContextSwitchOff();

=======
	context[currentTask].setContextSwitchOff();
	//contextSwitchFlag = false;
>>>>>>> e00d69394674b08fc7dac2d6b63da4f6a12306b4
}

int Parallelism::getContextSwitch()
{
<<<<<<< HEAD
	// return contextSwitchFlag;
	return context[currentTask].getContextSwitch();

=======
	return context[currentTask].getContextSwitch();
	//return contextSwitchFlag;
>>>>>>> e00d69394674b08fc7dac2d6b63da4f6a12306b4
}
//*
Task Parallelism::getContextTask(int taskNum)
{
	return context[taskNum];
}
//*

Task Parallelism::getContextTask(int currentTask)
{
	return context[currentTask];
}
void Parallelism::setProgInt()
// flag indicates to the extern function 'timerInterruptHandler'
// that this is an internal SMARTS software interrupt call,
// and the original BIOS function has no to be called.
{
	progInt = true;
}

void Parallelism::resetProgInt()
{
	progInt = false;
}

int Parallelism::getProgInt()
{
	return progInt;
}

void Parallelism::setEndOfTimeSlice()
// flag indicates that when 'context switch' will be enabled,
// it must also return the control to the scheduler.
{
	endOfTimeSlice = true;
}

char Parallelism::getName(int taskNum) // returns name found or ' ' if not
{
	return (taskNum <= totalTasks) ? context[taskNum].name : ' ';
}

char Parallelism::getCurrentName()
{
	return context[currentTask].name;
}

taskStatus Parallelism::getStatus(int taskNum)
// returns status or undefined if not found
{
	return (taskNum <= totalTasks) ? context[taskNum].status : UNDEFINED;
}

taskStatus Parallelism::getCurrentStatus()
{
	return context[currentTask].status;
}

void Parallelism::resume(int taskNum)
{
	if (taskNum < totalTasks)
		context[taskNum].status = READY;
}

void Parallelism::resume(char taskName)
{
	for (int i = 0; i < totalTasks; ++i)
		if (context[i].name == taskName)
			context[i].status = READY;
}

void Parallelism::setCurrentNotActive()
{
	context[currentTask].status = NOT_ACTIVE;
	--activeTasks;
	//*
	// Loop throw all tasks to see if everyone finished, because it
	// may be that activeTasks is 0 but the tasks still need to run.
	if (activeTasks == 0)
	{
		allFinished = true;
		for (int count = 0; count < getTotalTasks(); count++)
		{
			if (getCycles(count) > 0)
			{
				allFinished = false;
				break;
			}
		}
	}
	//*
}
void Parallelism::suspended()
{
	context[currentTask].status = SUSPENDED;
	callScheduler();
}

void Parallelism::incrPriority(int taskNum)
{
	if (taskNum < totalTasks)
	{
		context[taskNum].currentPriority = context[taskNum].currentPriority - 1;
	}
}
//*
void Parallelism::suspend(int taskNum){
	if (taskNum < totalTasks){
		context[taskNum].status = SUSPENDED;
		callScheduler();
	}
}
//*
//*
int Parallelism::getCurrentPriority(int taskNum)
{
	if (taskNum < totalTasks)
		return context[taskNum].currentPriority;
	else
	{
		return MAXINT;
	}
}
//*
//*
int Parallelism::getPriority(int taskNum)
{
	if (taskNum < totalTasks)
		return context[taskNum].priority;
	else
	{
		return MAXINT;
	}
}
//*
//*
void Parallelism::decreaseCycles(int taskNum)
{
	if (taskNum < totalTasks)
	{
		context[taskNum].currentCycles = context[taskNum].currentCycles - 1;
	}
}
//*
//*
int Parallelism::getCycles(int taskNum)
{
	if (taskNum < totalTasks)
		return context[taskNum].currentCycles;
	else
	{
		return 0;
	}
}
//*
//*
// Sets the error flag to alert for errors handling tasks.
void Parallelism::setError()
{
	error = true;
}
//*
//*
int Parallelism::getError()
{
	return error;
}
//*
//*
void Parallelism::reDeclare(int taskNum)
{
	if (taskNum < totalTasks)
	{
		context[taskNum].reDeclare();
	}
}
//*
void Parallelism::setOriginalPriority(int taskNum)
{
	if (taskNum < totalTasks)
	{
		context[taskNum].currentPriority = context[taskNum].priority;	}
}
void Parallelism::setPriority(int taskNum, int priority)
{
	context[taskNum].setPriority(priority);
}
void Parallelism::setCurrentOriginalPriority()
{
	context[currentTask].currentPriority = context[currentTask].priority;
}

Event *Parallelism::getExpectedEvent(int taskNum)
// returns *Event  or  NULL  if not found
{
	return (taskNum <= totalTasks) ? context[taskNum].expectedEvent : NULL;
}

Event *Parallelism::getCurrentExpectedEvent()
{
	return context[currentTask].expectedEvent;
}

void Parallelism::setCurrentExpectedEvent(Event *expectedEvent)
{
	context[currentTask].expectedEvent = expectedEvent;
}

void Parallelism::sleep(int t)
// Current task sleeps for 't' milliseconds
{
	if (t < MAXINT)
	{
		context[currentTask].sleepCount = t / 55 + 1;
		context[currentTask].status = SLEEP;
		++sleepTasks;
		callScheduler(); // return control to scheduler
	}
}

void Parallelism::sleepDecr(int taskNum)
{
	if (taskNum < totalTasks)
		context[taskNum].sleepDecr();
}

void Parallelism::getCurrentStack(unsigned &StackSeg, unsigned &StackPtr)
// Load current task stack pointer
{
	StackSeg = context[currentTask].stackSeg;
	StackPtr = context[currentTask].stackPtr;
}

void Parallelism::setCurrentStack(unsigned StackSeg, unsigned StackPtr)
// Save current task stack pointer
{
	context[currentTask].stackSeg = StackSeg;
	context[currentTask].stackPtr = StackPtr;
}

void Parallelism::getSchedStack(unsigned &StackSeg, unsigned &StackPtr)
// Load scheduler  stack pointer
{
	StackSeg = contextSched.stackSeg;
	StackPtr = contextSched.stackPtr;
}

// Essa funcao seria o relogio que nos queremos usar
void Parallelism::handleTimers()
// handling of the sleep status mode
{
	for (int i = totalTasks - 1; i >= 0; --i)
	{
		if (getStatus(i) == SLEEP)
		{
			sleepDecr(i);
			if (getStatus(i) == READY)
				--sleepTasks;
		}
	}
	//*
	
	// We loop throw the Tasks to decrease their periods, and
	// redeclare or set error when needed to.
	for (int count = 0; count < getTotalTasks(); count++)
	{
		if(getCycles(count) > 0)
		{
			if (getCurrentPriority(count) <= 0)
			{
				if (getStatus(count) != NOT_ACTIVE)
				{
					fprintf(myOutput, "\n\n\rError handling periodic tasks!");
					setError();
				}
				else{
					reDeclare(count);
				}
			}
			incrPriority(count);	// decrease period
		}
	}
	//*
}

void Parallelism::taskEnd()
// This function is called after the last operation of a task, instead of function return
{
	setCurrentNotActive();
	callScheduler(); // return the control to the scheduler to run a next task
}

/**********    class body:  Task     **********/
Task::Task()
{
	stack[MaxStack - 14] = _BP;
	stack[MaxStack - 13] = _DI;
	stack[MaxStack - 12] = _SI;
	stack[MaxStack - 11] = _DS;
	stack[MaxStack - 10] = _ES;
	stack[MaxStack - 9] = _DX;
	stack[MaxStack - 8] = _CX;
	stack[MaxStack - 7] = _BX;
	stack[MaxStack - 6] = _AX;
	stackSeg = FP_SEG(&stack[MaxStack - 14]);
	stackPtr = FP_OFF(&stack[MaxStack - 14]);
	status = NOT_ACTIVE;
	sleepCount = 0;
	currentPriority = priority = 0;
	currentCycles = cycles = 0;
	contextSwitchFlag = 1;
}
//-----------------------------------------------------

void Task::declare(void far *code, void far *userTaskEnd, char name, int period, int cycles)
{
	stack[MaxStack - 5] = FP_OFF(code);
	stack[MaxStack - 4] = FP_SEG(code);
	stack[MaxStack - 3] = _FLAGS;cycles--;
	stack[MaxStack - 2] = FP_OFF(userTaskEnd);
	stack[MaxStack - 1] = FP_SEG(userTaskEnd);
	this->name = name;
	this->priority = period;
	this->currentPriority = period;
	this->cycles = cycles;
	this->currentCycles = cycles;
	status = READY;
	contextSwitchFlag = 1;
	//*
	// We save the Task's stack so we can re-run it more times.
	for (int i = MaxStack - 1; i >= (MaxStack - 14); i--)
		copyStack[i] = stack[i];
	//*
}
// Redeclaring a Task gets back the original stack, decreases the cycles, resets
// the priority(period) and turn it status to ready.
void Task::reDeclare(){
	for (int i = MaxStack - 1; i >= (MaxStack - 14); i--)
		stack[i] = copyStack[i];
	// essas duas linhas foram copiadas do construtor de acordo com o professor
	stackSeg = FP_SEG(&stack[MaxStack - 14]);
	stackPtr = FP_OFF(&stack[MaxStack - 14]);

	currentCycles--;
	currentPriority = priority;
	status = READY;
	// contextSwitchFlag = 1;
}
int Task::getContextSwitch()
{
	return contextSwitchFlag;
}

void Task::setContextSwitchOff(){
	contextSwitchFlag = 0;
}
void Task::setContextSwitchOn(){
	contextSwitchFlag = 1;
}
//----------------------------------------------------
int Task::getContextSwitch()
{
	return contextSwitchFlag;
}

void Task::setContextSwitchOff()
{
	contextSwitchFlag = 0;
}
void Task::setContextSwitchOn()
{
	contextSwitchFlag = 1;
}
void Task::setPriority(int priority)
{
	this->priority = priority;
}
//----------------------------------------------------
void Task::sleepDecr()
// Decrements the sleep counter and update the task status accordingly
{
	if (status == SLEEP)
	{
		if (sleepCount > 0)
			--sleepCount;
		if (!sleepCount)
			status = READY;
	}
}

// FILE* myOutput = fopen("problem.txt", "w");
FILE* myOutput = fopen("solution.txt", "w");
