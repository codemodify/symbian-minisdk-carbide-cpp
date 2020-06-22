
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
* 	Part 7: Here is shown the ActiveObjects usage in a correct way.
* 			The difference from Part 6 is that here we create the "ActiveScheduler".
* 			ActiveScheduler is designed to manage the Active Objects and keeping
* 			them running.
* 
* 			This time the code indeed runs many "active objects' code" at the same time.
*			The output of this program beeing that there are 1000 'threads' instantiated,
* 			each printing its name.
* 
* 
*		Axiome to remember:
*		(o) Want to use "Light" threads called "Active Objects" - you need an CActiveScheduler to run them.
*
* 
* 
* 
* 
* 
*****************************************************************************/ 

// local headers
#include "commonfunctionalities/commonfunctionalities.h"
#include "commonfunctionalities/easyconsole/easyconsole.h"
#include "activeobject/activeobject.h"

// Symbian headers
#include <e32base.h> // for: CTrapCleanup, CleanupStack

struct Data
{
	EasyConsole& _consoleToUse;
	TInt _number;
};

void printDataThreaded( TAny* data )
{
	Data* incommingData = (Data*)data;
	if( 0 == incommingData )
		return;

	incommingData->_consoleToUse.write( Number(incommingData->_number) );
	incommingData->_consoleToUse.write( Text("\n") );

	return;
}

void runApplicationLogic( EasyConsole& consoleToUse )
{
	consoleToUse.write( Text("running the application's logic...\n") );
	
	// build the list of active objects
	ActiveObjectList* activeObjects = new ActiveObjectList( _FOFF(ActiveObject,iLink) );

	// the logic
	for( TInt index=0; index<1000; ++index )
	{
		Data* data = new Data();
		data->_consoleToUse = consoleToUse;
		data->_number = index;
		
		// doing the conversion number->string 
		TBuf<sizeof(TInt)> threadName;
		threadName.Num( index );

		// init a 'light thread' per outstanding operation
		ActiveObject* activeObject = new ActiveObject();
		activeObject->setCallback( printDataThreaded, data );
		
		// add the 'light thread' to monitored list
		activeObjects->AddLast( *activeObject );
	}
	
	// add a special active objects that will check the list of the outstanding ones.
	// This active object will be the one to call "CActiveScheduler::Stop()"
	TCallBack callBack( checkForAnyActiveObjects, activeObjects );
	CIdle* idle = CIdle::New( CActive::EPriorityIdle );
	idle->Start( callBack );
}

int theThrowingStuff()
{
			EasyConsole* easyConsole = new EasyConsole();
			if( 0 == easyConsole )
				return -1;

			CleanupStack::PushL( easyConsole );

	// ACTIVE_SCHEDULER: init the manager for active objects
	CActiveScheduler* activeScheduler = new (ELeave) CActiveScheduler();
	CleanupStack::PushL( activeScheduler );
	CActiveScheduler::Install( activeScheduler );
	
			easyConsole->write( Text("console initialized and running...\n") ); 
	
	// ACTIVE_SCHEDULER: let the Active Objects start their outstanding requests
	runApplicationLogic( *easyConsole );

	// ACTIVE_SCHEDULER: start the manager so he can manage its active objects
	// this is a blocking funciton, it will exit from it only after some-one calls 
	// the "CActiveScheduler::Start()" 
	CActiveScheduler::Start();
	
			easyConsole->write( Text("the application has finished its actions...\n") );
			easyConsole->waitKeyPress();

	// ACTIVE_SCHEDULER: cleanup the active objects sheduler
	CleanupStack::PopAndDestroy( activeScheduler );
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
