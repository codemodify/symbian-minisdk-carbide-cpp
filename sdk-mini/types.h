#ifndef __Types_H__
#define __Types_H__

// system headers
#include <e32std.h>
#include <e32const.h>

// used to commend out the unused parameters to the functions, to kill the warnings
#define UNUSED( parameter )



// strings 
typedef TBuf<1000>	String1;
typedef TBuf<2000>	String2;
typedef TBuf<3000>	String3;
typedef TBuf<4000>	String4;
typedef TBuf<5000>	String5;
typedef TBuf<6000>	String6;
#define String		String6
#define TEXT( t )	_L( t ) // I believe this is some trash in the brains inherited from Win32,
							// as well as the illusion of Symbian's cross platformity which
							// not only that can't run on different platforms, it can't even
							// run from one release to another on their devices.

// numbers
typedef TInt		Int;
typedef TReal		Real;



// complimentary types
typedef TRect		Rect;
typedef TPoint		Point;

typedef TBool		Bool;
#define True		ETrue
#define False		EFalse
typedef TAny  		Void;

Int operator == ( TTrue, int );
Int operator == ( TFalse, int );



// data back-and-forth pushers
Int StringToInt( String& theString );
Real StringToReal( String& theString );

void IntToString( Int theInt, String& theString, Bool clearDoNotAppend = True );
void RealToString( Real theReal, String& theString, Bool clearDoNotAppend = True );



// add support for delegating
#include "delegates.h"



#endif
