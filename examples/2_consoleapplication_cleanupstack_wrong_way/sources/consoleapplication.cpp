
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
* 	Part 2: Here is shown the CleanupStack usage in a wrong way.
* 			Despite the fact that the code compiles and has logic and
* 			normally is expected to work on runtime - it does not.
* 			Reason: the "CleanupStack" has an internal pointer to the TRAP()
* 			that was supposed to 'wrap' the PushL() operation. If you try to
* 			run this app you wil get a panic and eventually the app crashes.
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
*  
* 
*****************************************************************************/ 

// local headers
#include "commonfunctionalities/commonfunctionalities.h"
#include "commonfunctionalities/easyconsole/easyconsole.h"

// Symbian headers
#include <e32base.h> // for: CTrapCleanup, CleanupStack

void runApplicationLogic( EasyConsole& consoleToUse )
{
	consoleToUse.write( Text("running the application's logic...\n") );

	// the logic
	for( TInt index=0; index<20; ++index )
	{
		consoleToUse.write( Number(index) );
		consoleToUse.write( Text("\n") );
	}
}

int__main()
{
	// CLEANUP_STACK: creating the cleanup stack
	CTrapCleanup* cleanupStack = CTrapCleanup::New();
	
		EasyConsole* easyConsole = new EasyConsole();
		if( 0 == easyConsole )
			return -1;
		
	// CLEANUP_STACK: mark an object for delete
	CleanupStack::PushL( easyConsole );
		
		easyConsole->write( Text("console initialized and running...\n") ); 
	
		runApplicationLogic( *easyConsole );
	
		easyConsole->write( Text("the application has finished its actions...\n") );
		easyConsole->waitKeyPress();
	

	// CLEANUP_STACK: destroy the object
	CleanupStack::PopAndDestroy( easyConsole );

	// CLEANUP_STACK: destroy the cleanup stack
	delete cleanupStack;

	return 0;
}
