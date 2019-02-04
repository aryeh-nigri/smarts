/********************************/
/*   Aryeh Nigri	338017866	*/
/*   Zeev Noiman	937574		*/
/********************************/

/********************************************************************/
/* A Small Real Time System for the Real-Time laboratory            */
/* built by: A.Teitelbaum on an idea of H.G.Mendelbaum              */
/* Jerusalem College of Technology, 5759-64 (1999-03)               */
/* update  Tishrey   5777                                           */
/* APP77.CPP, an application to demonstrate SMARTS77  functioning   */
/********************************************************************/
#include "SMARTS77.H"
#include "Mutex.h"

// Mutex m1(0, "M1"), m2(0, "M2");	// for problem.txt, send 0 for priorityInheritanceFlag
Mutex m1(1, "M1"), m2(1, "M2");		// for solution.txt, send 1 for priorityInheritanceFlag

Mutex mA(0, "MA"), mB(0, "MB"), mC(0, "MC");
Mutex mD(0, "MD"), mE(0, "ME"), mF(0, "MF");

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
	// fprintf(myOutput, "\nTask D acquiring M2\n");
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
	// fprintf(myOutput, "\nTask D releasing M2\n");
	m2.release();

	// this for is to see the difference when we reset the priority inherited from F
	for (long i = 0; i < 600000; i++);//600.000 3 time clock
	
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
	// fprintf(myOutput, "\nTask F acquiring M2\n");
	m2.acquire();
	for (int j = 0; j < 10; j++)
	{
		for (long i = 0; i < 600000; i++);
		// m.acquire();
		fprintf(myOutput, "F");
		// m.release();
	}
	// fprintf(myOutput, "\nTask F releasing M2\n");
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
	
	SMARTS.declareTask(d, 'D', 302, 1);
	SMARTS.declareTask(e, 'E', 301, 1);
	SMARTS.declareTask(f, 'F', 300, 1);

	fprintf(myOutput, "/********************************/\n");
	fprintf(myOutput, "/*   Aryeh Nigri	338017866	*/\n");
	fprintf(myOutput, "/*   Zeev Noiman	937574		*/\n");
	fprintf(myOutput, "/********************************/\n\n");

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