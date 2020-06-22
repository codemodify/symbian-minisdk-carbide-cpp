
#include <ImageConversion.h>
#include <EIKENV.H>

// local headers
#include "ImageManipulator.h"

ImageManipulator::ImageManipulator() :
	CActive( CActive::EPriorityHigh )
{
    CActiveScheduler::Add( this );

    _bitmapRotator = CBitmapRotator::NewL();
}
 
ImageManipulator::~ImageManipulator()
{
	if( IsActive() )
		Cancel();

	if( _bitmapRotator )
    {
        delete _bitmapRotator;
        _bitmapRotator = 0;
    }
}

void ImageManipulator::DoCancel()
{
	if( _bitmapRotator )
		_bitmapRotator->Cancel();
}
 
void ImageManipulator::RunL()
{
	if( _manipulation && _manipulationDelegate )
		(_manipulation->*_manipulationDelegate)(  eRotate  );
}
 
TInt ImageManipulator::RunError( TInt aError )
{
    return KErrNone;
}

void ImageManipulator::Rotate( CFbsBitmap& bitmap, CBitmapRotator::TRotationAngle angle )
{
 	if( _bitmapRotator )
	{
 		if( !IsActive() )
 		{
 			SetActive();
 			_bitmapRotator->Rotate( &iStatus, bitmap, angle );
 		}
	}
}

void ImageManipulator::SaveImage( Int bitmapHandle, String& filePath )
{
	_bitmap = new CFbsBitmap();
	_bitmap->Duplicate( bitmapHandle );
	
	RFs& fs = CEikonEnv::Static()->FsSession();

	const TDesC p( filePath );
	
//	String fileExt;
//	fileExt.Append( TEXT("image/jpeg") );
//	const TDesC8 p2( fileExt );

	_LIT8( shittyWayToPointAmimeType, "image/jpeg" );
	CImageEncoder* imageEncoder = CImageEncoder::FileNewL(	fs,
															p,
															shittyWayToPointAmimeType );
	imageEncoder->Convert( &iStatus, *_bitmap );
}
