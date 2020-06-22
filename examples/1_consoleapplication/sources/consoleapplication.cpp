
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
* 	Part 1: Here is shown a simple console application that does not use much
* 			Symbian's concepts, and has a logic and look for a traditional app.
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
* 
* 
* 
* 
*****************************************************************************/ 

// local headers
#include "commonfunctionalities/commonfunctionalities.h"
#include "commonfunctionalities/easyconsole/easyconsole.h"




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
	EasyConsole* easyConsole = new EasyConsole();
	if( 0 == easyConsole )
		return -1;
	
	easyConsole->write( Text("console initialized and running...\n") ); 

	runApplicationLogic( *easyConsole );

	easyConsole->write( Text("the application has finished its actions...\n") );
	easyConsole->waitKeyPress();
	delete easyConsole;	

	return 0;
}


