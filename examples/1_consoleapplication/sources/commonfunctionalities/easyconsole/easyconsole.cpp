
// Symbian headers
#include <e32cons.h> // for: CConsoleBase

// local headers
#include "easyconsole.h"

static CConsoleBase* s_console = 0;

EasyConsole::EasyConsole()
{
	// because of the Symbians speciffics, the 'C' prefixed classes must be created on the heap
	TRAPD( errorOccured, s_console=Console::NewL(_L(""),TSize(KConsFullScreen,KConsFullScreen)) ) 
	if( KErrNone != errorOccured )
		return;
}

EasyConsole::~EasyConsole()
{
	if( 0 != s_console )
	{
		delete s_console;
		s_console = 0;
	}
}

void EasyConsole::write( const TDesC& dataToWrite )
{
	if( !s_console )
		return;

	s_console->Write( dataToWrite );
}

void EasyConsole::write( const TInt& dataToWrite )
{
	if( !s_console )
		return;

	// doing the conversion number->string 
	TBuf<sizeof(TInt)> buffer;
	buffer.Num( dataToWrite );

	s_console->Write( buffer );
}

void EasyConsole::write( const TReal& dataToWrite )
{
	if( !s_console )
		return;

	// doing the conversion number->string
	TBuf<sizeof(TInt)> buffer;
	buffer.Num( dataToWrite );

	s_console->Write( buffer );
}

void EasyConsole::waitKeyPress()
{
	if( !s_console )
		return;

	s_console->Getch();
}
