#ifndef __CameraEngine_H__
#define __CameraEngine_H__

#include <e32base.h>
#include <ecam.h>
#include <coecntrl.h>

#include "sources/sdk-mini/types.h"
#include "sources/sdk-mini/CameraEngine/ImageManipulator.h"

class CameraEngine :	public MCameraObserver,
						public ImageManipulator::Manipulation		// used to do async image manipulations 
{
	public:
		struct ImageShot{	typedef void (ImageShot::*Delegate)( CFbsBitmap* bitmap );			};
		ImageShot*			_imageShot;
		ImageShot::Delegate	_imageShotDelegate;
		
		struct VideoFrame{	typedef void (VideoFrame::*Delegate)( MFrameBuffer* frameBuffer );	};
		VideoFrame*				_videoFrame;
		VideoFrame::Delegate	_videoFrameDelegate;
	
	// some traditional stuff
    public:
    	CameraEngine();
        virtual ~CameraEngine();

	// types used itnernally and extermnally 
    public:
    	typedef TCameraInfo CameraInfo;	// The 'T' from borland is not a such a good idea
    									// the example being that now the guy works for MS
    									// and does C# where types start with their real names
    									// ex: System, Clock, FuckStupidSymbianTypes.
    									// The cryptology usually lives well within a time frame
    									// of the mathematician's moment of writing the thing.
    	
    // interfaces for camera settings
    public:
    	void TakeShot();
    	
//    	void SetParams_ImageSize			( const TSize& imageSize );
    	void SetParams_ZoomFactor			( const Int& zoomFactor );
    	void SetParams_DigitalZoomFactor	( const Int& digitalZoomFactor );
    	void SetParams_Contrast				( TInt contrast );
    	void SetParams_Brightness			( TInt brightness );
    	void SetParams_Flash				( CCamera::TFlash flashMode );
    	void SetParams_Exposure				( CCamera::TExposure exposure );
    	void SetParams_WhiteBalance			( CCamera::TWhiteBalance whiteBalance );
    	void SetParams_Landscape			();
    	void SetParams_Portrait				();

//    	void GetParams_ImageSize( TSize& imageSize );
    	void GetParams_ZoomFactor( Int& zoomFactor );
    	void GetParams_ZoomFactorMin( Int& zoomFactorMin );
    	void GetParams_ZoomFactorMax( Int& zoomFactorMax );
    	void GetParams_DigitalZoomFactor( Int& digitalZoomFactor );
    	void GetParams_DigitalZoomFactorMin( Int& digitalZoomFactorMin );
    	void GetParams_DigitalZoomFactorMax( Int& digitalZoomFactorMax );
    	void GetParams_Contrast( Int& contrast );
    	void GetParams_ContrastMin( Int& contrastMin );
    	void GetParams_ContrastMax( Int& contrastMax );
    	void GetParams_Brightness( Int& brightness );
    	void GetParams_BrightnessMin( Int& brightnessMin );
    	void GetParams_BrightnessMax( Int& brightnessMax );
    	void GetParams_Flash( Int& flash );
    	void GetParams_FlashMin( Int& flash );
    	void GetParams_FlashMax( Int& flash );
    	void GetParams_Exposure( Int& exposure );
    	void GetParams_ExposureMin( Int& exposure );
    	void GetParams_ExposureMax( Int& exposure );
    	void GetParams_WhiteBalance( Int& whiteBalance );
    	void GetParams_WhiteBalanceMin( Int& whiteBalance );
    	void GetParams_WhiteBalanceMax( Int& whiteBalance );
    	

    // interfaces for general manipulation
    public:
    	TBool IsCameraDetected();
    	
		void StartViewFinder();
		void StartViewFinder( CCoeControl* control );
		void StopViewFinder();
		
		void SaveImage( Int bitmapHandle, String& imagPath ){ _imageManipulator->SaveImage(bitmapHandle,imagPath); }

		const CameraInfo& GetCameraInfo();
		void GetCameraInfoAsString( String& string );

	// Functions from MCameraObserver
	public:

		// This method informs that camera reservation is complete.
		// Called asynchronously when CCamera::Reserve() completes.
	 	void ReserveComplete( TInt aError );
	 	
	 	// This method indicates camera power on is complete.
		// Called on completion of CCamera:PowerOn().
		void PowerOnComplete( TInt aError );

		// This method tests whether transfer of view finder data has completed.
		// Called periodically in response to the use of CCamera::StartViewFinderBitmapsL().
		void ViewFinderFrameReady( CFbsBitmap& frame );

		// This method transfers the current image from the camera to the client.
		// Called asynchronously when CCamera::CaptureImage() completes.
		void ImageReady( CFbsBitmap* bitmap, HBufC8* data, TInt error );
		
		// This method passes a filled frame buffer to the client.
		// Called asynchronously, when a buffer has been filled with the required number 
		// of video frames by CCamera::StartVideoCapture().
		void FrameBufferReady( MFrameBuffer* frameBuffer, TInt error );
		
	// Interfaces used to handle operations from "ImageManipulator" 
	private:
		void ManipulationComplete( ImageManipulator::ImageOperation imageOperation );

	// Interfaces for internal usage
	private:
		void DrawImageOnControl();


	// Data
	private:
		CCamera*		_camera;
		CameraInfo*		_cameraInfo;
//		TSize			_imageSize;
		
		CCoeControl*	_controlToDrawOn;
		
		Bool				_isPortrait;
		CFbsBitmap			_currentViewFinderImage;
		ImageManipulator*	_imageManipulator; 
		
	    enum FinderState
	    {
	    	eFinderInactive = 0,
			eFinderInitializing, 
			eFinderFailed,
			eFinderActive,
			eFinderTakingPicture
		};
		FinderState	_cameraState;
};

#endif // __CameraEngine_H__
