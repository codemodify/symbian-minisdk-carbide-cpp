#ifndef easy_console_h
#define easy_console_h

// Symbian headers
#include <e32def.h> // for: TInt, TReal
#include <e32cmn.h> // for: TDesC

/*****************************************************************************
* 
*	This class is designed to facilitate access to the Symbian's console.
* 
*****************************************************************************/ 
class EasyConsole
{
	public:
		EasyConsole();
		~EasyConsole();

	// intrefaces to use
	public:
		void write( const TDesC& dataToWrite );
		void write( const TInt& dataToWrite );
		void write( const TReal& dataToWrite );
		void waitKeyPress();

};

#endif
