#ifndef __ImageManipulator_H__
#define __ImageManipulator_H__

// system headers
#include <e32base.h>
#include <BitmapTransforms.h>

#include <sdk-mini/types.h>

class ImageManipulator : public CActive
{
	public:
		ImageManipulator();
		virtual ~ImageManipulator();

	typedef enum{ eRotate, eSave, eDone } ImageOperation; 

	struct Manipulation{ typedef void (Manipulation::*Delegate)( ImageOperation imageOperation );	};
	Manipulation* _manipulation;
	Manipulation::Delegate _manipulationDelegate;


	// from CActive
	private: 
		void DoCancel();
		void RunL();
		TInt RunError(TInt aError);

	// new functions
	public:
		
		// Rotates a bitmap by a specified angle.
		//
		// Schedules an asynchronous rotate operation with specified degrees on the bitmap.
		// Overwrites the original bitmap with the new rotated image. 
		void Rotate( CFbsBitmap& bitmap, CBitmapRotator::TRotationAngle angle );
		
		// Schedules an asynchronous save request (because this supid fuckedup symbian otherwise can't work)
		void SaveImage( Int bitmapHandle, String& filePath );

	// Data
	private:
		CBitmapRotator* _bitmapRotator;
		CFbsBitmap*		_bitmap;
};

#endif // __ImageManipulator_H__
