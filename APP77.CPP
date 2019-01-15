/********************************************************************/
/* A Small Real Time System for the Real-Time laboratory            */
/* built by: A.Teitelbaum on an idea of H.G.Mendelbaum              */
/* Jerusalem College of Technology, 5759-64 (1999-03)               */
/* update  Tishrey   5777                                           */
/* APP77.CPP, an application to demonstrate SMARTS77  functioning   */
/********************************************************************/
#include "SMARTS77.H"
#include "Mutex.h"

Mutex m1(1), m2(1);
Mutex mA, mB, mC;
Mutex mD, mE, mF;
Event AB, AC;
Event DE, DF;

// user Functions
void a()
{
	mA.acquire();
	fprintf(myOutput, "\n *************   A Start    *********************\n");
	mA.release();
	// fprintf(myOutput, "\nTask A acquiring M1\n");
	m1.acquire();
	fprintf(myOutput, "\nTask A sending events\n");
	AC.send('C', NULL, 0);
	AB.send('B', NULL, 0);
	// test for leveling mutex
	// m1.acquire();
	// m1.release();
	for (int j = 0; j < 50; j++)
	{
		for (long i = 0; i < 600000; i++);//600.000 3 time clock
		// m.acquire();
		fprintf(myOutput, "A");
		// m.release();
	}
	// fprintf(myOutput, "\nTask A releasing M1\n");
	m1.release();
	mA.acquire();
	fprintf(myOutput, "\n *************   A Finish   *********************\n");
	mA.release();
}

void b()
{
	mB.acquire();
	fprintf(myOutput, "\n *************   B Start    *********************\n");
	mB.release();
	fprintf(myOutput, "\nTask B waiting event\n");
	AB.wait('A');
	fprintf(myOutput, "\nTask B received event\n");
	for (int j = 0; j < 50; j++)
	{
		for (long i = 0; i < 600000; i++);
		mB.acquire();
		fprintf(myOutput, "B");
		mB.release();
	}
	mB.acquire();
	fprintf(myOutput, "\n *************   B Finish   *********************\n");
	mB.release();
}

void c()
{
	mC.acquire();
	fprintf(myOutput, "\n *************   C Start    *********************\n");
	mC.release();
	fprintf(myOutput, "\nTask C waiting event\n");
	AC.wait('A');
	fprintf(myOutput, "\nTask C received event\n");
	// fprintf(myOutput, "\nTask C acquiring M1\n");
	m1.acquire();
	for (int j = 0; j < 50; j++)
	{
		for (long i = 0; i < 600000; i++);
		// m.acquire();
		fprintf(myOutput, "C");
		// m.release();
	}
	// fprintf(myOutput, "\nTask C releasing M1\n");
	m1.release();
	mC.acquire();
	fprintf(myOutput, "\n *************   C Finish   *********************\n");
	mC.release();
}

void d()
{
	mD.acquire();
	fprintf(myOutput, "\n *************   D Start    *********************\n");
	mD.release();
	fprintf(myOutput, "\nTask D acquiring M2\n");
	m2.acquire();
	fprintf(myOutput, "\nTask D sending events\n");
	DF.send('F', NULL, 0);
	DE.send('E', NULL, 0);
	for (int j = 0; j < 10; j++)
	{
		for (long i = 0; i < 600000; i++);//600.000 3 time clock
		// m.acquire();
		fprintf(myOutput, "D");
		// m.release();
	}
	fprintf(myOutput, "\nTask D releasing M2\n");
	m2.release();
	mD.acquire();
	fprintf(myOutput, "\n *************   D Finish   *********************\n");
	mD.release();
}

void e()
{
	mE.acquire();
	fprintf(myOutput, "\n *************   E Start    *********************\n");
	mE.release();
	fprintf(myOutput, "\nTask E waiting event\n");
	DE.wait('D');
	fprintf(myOutput, "\nTask E received event\n");
	for (int j = 0; j < 10; j++)
	{
		for (long i = 0; i < 600000; i++);
		mE.acquire();
		fprintf(myOutput, "E");
		mE.release();
	}
	mE.acquire();
	fprintf(myOutput, "\n *************   E Finish   *********************\n");
	mE.release();
}

void f()
{
	mF.acquire();
	fprintf(myOutput, "\n *************   F Start    *********************\n");
	mF.release();
	fprintf(myOutput, "\nTask F waiting event\n");
	DF.wait('D');
	fprintf(myOutput, "\nTask F received event\n");
	fprintf(myOutput, "\nTask F acquiring M2\n");
	m2.acquire();
	for (int j = 0; j < 10; j++)
	{
		for (long i = 0; i < 600000; i++);
		// m.acquire();
		fprintf(myOutput, "F");
		// m.release();
	}
	fprintf(myOutput, "\nTask F releasing M2\n");
	m2.release();
	mF.acquire();
	fprintf(myOutput, "\n *************   F Finish   *********************\n");
	mF.release();
}

void main()
{
	clrscr();
	// SMARTS.externalFunctions(timerInterruptHandler, scheduler, myTaskEnd, roundRobin);
	// SMARTS.externalFunctions(timerInterruptHandler, scheduler, myTaskEnd, EDF);
	SMARTS.externalFunctions(timerInterruptHandler, scheduler, myTaskEnd, RMS);
	SMARTS.declareTask(a, 'A', 402, 1);
	SMARTS.declareTask(b, 'B', 401, 1);
	SMARTS.declareTask(c, 'C', 400, 1);
	
	// SMARTS.declareTask(d, 'D', 300, 1);
	// SMARTS.declareTask(e, 'E', 301, 1);
	// SMARTS.declareTask(f, 'F', 302, 1);

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





/*

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

*/