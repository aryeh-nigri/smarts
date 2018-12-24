/********************************************************************/
/* A Small Real Time System for the Real-Time laboratory            */
/* built by: A.Teitelbaum on an idea of H.G.Mendelbaum              */
/* Jerusalem College of Technology, 5759-64 (1999-03)               */
/* update  Tishrey   5777                                           */
/* APP77.CPP, an application to demonstrate SMARTS77  functioning   */
/********************************************************************/
#include "smarts77.h"

// user Functions
void a()
{
	SMARTS.contextSwitchOff();
	fprintf(myOutput, "\n *************   A Start    *********************\n");
	// cout << "\n *************   A Start    *********************\n";
	SMARTS.contextSwitchOn();
	for (int j = 0; j < 10; j++)
	{
		for (long i = 0; i < 600000; i++);//600.000 3 time clock
		SMARTS.contextSwitchOff();
		fprintf(myOutput, "A");
		// cout << "A";//<<j;
		SMARTS.contextSwitchOn();
	}
	SMARTS.contextSwitchOff();
	fprintf(myOutput, "\n *************   A Finish   *********************\n");
	// cout << "\n *************   A Finish   *********************\n";
	SMARTS.contextSwitchOn();
}

void b()
{
	SMARTS.contextSwitchOff();
	fprintf(myOutput, "\n *************   B Start    *********************\n");
	// cout << "\n *************   B Start    *********************\n";
	SMARTS.contextSwitchOn();
	for (int j = 0; j < 10; j++)
	{
		for (long i = 0; i < 600000; i++);
		SMARTS.contextSwitchOff();
		fprintf(myOutput, "B");
		// cout << "B";
		SMARTS.contextSwitchOn();
	}
	SMARTS.contextSwitchOff();
	fprintf(myOutput, "\n *************   B Finish   *********************\n");
	// cout << "\n *************   B Finish   *********************\n";
	SMARTS.contextSwitchOn();
}

void c()
{
	SMARTS.contextSwitchOff();
	fprintf(myOutput, "\n *************   C Start    *********************\n");
	// cout << "\n *************   C Start    *********************\n";
	SMARTS.contextSwitchOn();
	for (int j = 0; j < 10; j++)
	{
		for (long i = 0; i < 600000; i++);
		SMARTS.contextSwitchOff();
		fprintf(myOutput, "C");
		// cout << "C";
		SMARTS.contextSwitchOn();
	}
	SMARTS.contextSwitchOff();
	fprintf(myOutput, "\n *************   C Finish   *********************\n");
	// cout << "\n *************   C Finish   *********************\n";
	SMARTS.contextSwitchOn();
}

void main()
{
	clrscr();
	// SMARTS.externalFunctions(timerInterruptHandler, scheduler, myTaskEnd, roundRobin);
	// SMARTS.externalFunctions(timerInterruptHandler, scheduler, myTaskEnd, EDF);
	SMARTS.externalFunctions(timerInterruptHandler, scheduler, myTaskEnd, RMS);
	// SMARTS.declareTask(a, 'A', 30, 1);
	// SMARTS.declareTask(b, 'B', 70, 1);
	// SMARTS.declareTask(c, 'C', 100, 1);
	SMARTS.declareTask(a, 'A', 30, 5);
	SMARTS.declareTask(b, 'B', 70, 3);
	SMARTS.declareTask(c, 'C', 100, 2);
	SMARTS.runTheTasks();
}

/*

1) Work only with EDF:
	SMARTS.declareTask(a, 'A', 50, 2);
	SMARTS.declareTask(b, 'B', 70, 3);
	SMARTS.declareTask(c, 'C', 100, 4);

OUTPUT RR:
*************   B Start    *********************

*************   C Start    *********************

*************   A Start    *********************
CABCABCABCABCABCABCAB


2) Work with RR and EDF:
	SMARTS.declareTask(a, 'A', 80, 3);
	SMARTS.declareTask(b, 'B', 120, 3);
	SMARTS.declareTask(c, 'C', 180, 3);

3) Dont work with RR and EDF:
	SMARTS.declareTask(a, 'A', 30, 5);
	SMARTS.declareTask(b, 'B', 70, 3);
	SMARTS.declareTask(c, 'C', 100, 2);

OUTPUT RR:
*************   B Start    *********************

*************   C Start    *********************

*************   A Start    *********************
CABCABCABCAB

OUTPUT EDF:
*************   A Start    *********************
AAAA

*/