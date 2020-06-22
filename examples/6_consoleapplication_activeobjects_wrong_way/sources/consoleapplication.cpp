
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
* 	Part 6: Here is shown the ActiveObjects usage in a wrong way.
* 			The difference from Part 7 is that here we create the Active Objects" only
* 			and expect them to do their job.
* 
*  
* 			This time the code is trying to run many "active objects' code" at the same time.
*			The output of this program beeing nothing relevat that the active objects do
*			from within their code.
* 
*
* 
*  
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
//	TCallBack callBack( checkForAnyActiveObjects, activeObjects );
//	CIdle* idle = CIdle::New( CActive::EPriorityIdle );
//	idle->Start( callBack );
}

int theThrowingStuff()
{
	EasyConsole* easyConsole = new EasyConsole();
	if( 0 == easyConsole )
		return -1;

	CleanupStack::PushL( easyConsole );

	
	
	
	
	easyConsole->write( Text("console initialized and running...\n") ); 

	
	runApplicationLogic( *easyConsole );





	
	easyConsole->write( Text("the application has finished its actions...\n") );
	easyConsole->waitKeyPress();



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
