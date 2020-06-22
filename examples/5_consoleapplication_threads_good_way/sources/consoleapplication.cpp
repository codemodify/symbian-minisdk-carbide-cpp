
/*****************************************************************************
* 
*	This file shows a simple console application that resembles 
*	a C/C++ program.
*
*	It is intended to help and smooth the migration process 
*	from C/C++ way to Symbian's way of coding.
*
*	Next few examples shall start from here and change more till
*	we'll get to Symbian coding styles and show why they took this path.
*
* 	Part 3: Here is shown the CleanupStack usage in a good way.
* 			The difference from Part 2 is that here we assign a TRAP()
* 			for the PushL() by moving all that functionality into a another
* 			function.
* 
* 			This time the code compiles and at runtime it does not
* 			throws any surprizes that Symbian so carefully designed to take care of.
* 			Hooray!.
* 
*		Axiome to remember:
*		(o) For any PushL() you should have atleast one TRAP() enclosing that PushL().
*		(o) All items pushed within a TRAP should be popped out before coming out of TRAP().
*
*		Otherwise your app becomes a unexperienced little child and panics right a way,
* 		its cryings are heard even by the nanokernel and it pops you a message box saying
* 		that the app is badly written.
*  
* 
*****************************************************************************/ 

// local headers
#include "commonfunctionalities/commonfunctionalities.h"
#include "commonfunctionalities/easyconsole/easyconsole.h"

// Symbian headers
#include <e32base.h> // for: CTrapCleanup, CleanupStack
#include <e32std.h>  // for: RThread, RMutex

struct Data
{
	EasyConsole* _consoleToUse;
	TInt _number;
};

TInt printDataThreaded( TAny* data )
{
	Data* incommingData = (Data*)data;
	if( 0 == incommingData )
		return 0;

	RSemaphore semaphore;
	semaphore.OpenGlobal( Text("TheSemaphore") );

	incommingData->_consoleToUse->write( Number(incommingData->_number) );
	incommingData->_consoleToUse->write( Text("\n") );

	semaphore.Signal();
	semaphore.Close();
	
	return 0;
}

void runApplicationLogic( EasyConsole* consoleToUse )
{
		consoleToUse->write( Text("running the application's logic...\n") );

	// create active scheduler (to run active objects)
	CActiveScheduler* activeScheduler = new (ELeave) CActiveScheduler();
	CleanupStack::PushL( activeScheduler );
	CActiveScheduler::Install( activeScheduler );
	
		// the logic
		for( TInt index=0; index<20; ++index )
		{
			Data* data = new Data();
			data->_consoleToUse = consoleToUse;
			data->_number = index;
			
			// doing the conversion number->string 
			TBuf<sizeof(TInt)> threadName;
			threadName.Num( index );
			
			// FIXME: this shit is temporary here
			printDataThreaded( data );
			
			RThread thread;
			TInt result = thread.Create( threadName, (TThreadFunction)printDataThreaded, KDefaultStackSize, KMinHeapSize, 256*KMinHeapSize, data, EOwnerThread );
			if( KErrNone == result )
				thread.Resume();
			else
				result = 123123;
		}

	// delete active scheduler
	CleanupStack::PopAndDestroy( activeScheduler );
}

int theThrowingStuff()
{
		EasyConsole* easyConsole = new EasyConsole();
		if( 0 == easyConsole )
			return -1;
		
		// CLEANUP_STACK: mark an object for delete
		CleanupStack::PushL( easyConsole );
		
		easyConsole->write( Text("console initialized and running...\n") ); 

	RSemaphore semaphore;
	semaphore.CreateGlobal( Text("TheSemaphore"), 0 );

		runApplicationLogic( easyConsole );
	
		easyConsole->write( Text("the application has finished its actions...\n") );
		easyConsole->waitKeyPress();

	semaphore.Wait();
	semaphore.Close();
		
		// CLEANUP_STACK: destroy the object
		CleanupStack::PopAndDestroy( easyConsole );
	
		return KErrNone;
}

int__main()
{
	// CLEANUP_STACK: creating the cleanup stack
	CTrapCleanup* cleanupStack = CTrapCleanup::New();
	
	int errorCode = KErrNone;
	TRAP( errorCode, theThrowingStuff() );
	if( KErrNone != errorCode )
		; // something wrong went here

	// CLEANUP_STACK: destroy the cleanup stack
	delete cleanupStack;

	return 0;
}
