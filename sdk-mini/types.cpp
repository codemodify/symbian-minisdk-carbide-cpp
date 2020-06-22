
#include "types.h"


//
// Implementation follows. Was it that hard in decades of wasted money to have 
// a human interface to the platform that would have offered a feature to the platform ?
// Don't know how to ? Check what Qt is. Know how to ? Check what Qt is.
//_______________________________________________________________________________________

// cracking wide open
Int operator == ( TTrue t, int i ){ return ( ((int)t) == i ); }
Int operator == ( TFalse f, int i ){ return ( ((int)f) == i ); }

Int StringToInt( String& theString )
{
	TInt output;
	
	TLex theLexer( theString );
	theLexer.Val( output );
	
	return output; 
}

Real StringToReal( String& theString )
{
	TReal output;
	
	TLex theLexer( theString );
	theLexer.Val( output );
	
	return output; 	
}


void IntToString( Int theInt, String& theString, Bool clearDoNotAppend )
{
	if( True == clearDoNotAppend )
		theString.Zero();
	
	theString.AppendNum( theInt );
}

void RealToString( Real theReal, String& theString, Bool clearDoNotAppend )
{
	if( True == clearDoNotAppend )
		theString.Zero();
	
	TRealFormat realFormat( 8, 5 );
	realFormat.iType = KRealFormatFixed | KDoNotUseTriads;

	theString.AppendNum( theReal, realFormat );
}
