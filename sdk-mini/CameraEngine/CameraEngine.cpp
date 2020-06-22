
#include "CameraEngine.h"
//#include <coemain.h>

//static CFbsBitmap s_viewFinderImageDummy;

CameraEngine::CameraEngine() :
	_camera( 0 ),
	_cameraInfo( 0 ),
	_cameraState( eFinderInactive ),
	_imageShot( 0 ),
	_imageShotDelegate( 0 ),
	_videoFrame( 0 ),
	_videoFrameDelegate( 0 ),
	_isPortrait( True ),
	_imageManipulator( 0 ),
	_controlToDrawOn( 0 )
//	_currentViewFinderImage( s_viewFinderImageDummy )
//	_currentViewFinderImage( reinterpret_cast<CFbsBitmap&>(void*) )
{
	if( IsCameraDetected() )
	{
		_camera = CCamera::NewL( *this, 0 );

		GetCameraInfo();
	}
}

CameraEngine::~CameraEngine()
{
    if( _camera )
    {
    	StopViewFinder();
    	
	    delete _camera;
	    _camera = 0;
	    
	    delete _cameraInfo;
	    _cameraInfo = 0;
    }
}

void CameraEngine::TakeShot()
{
	_cameraState = eFinderTakingPicture;
	_camera->StopViewFinder();
	_camera->CaptureImage();
}

//------------------------------------------------------
//	Setters
//------------------------------------------------------
//void CameraEngine::SetParams_ImageSize( const TSize& imageSize )
//{
//	if( !IsCameraDetected() )
//		return;
//
//	_imageSize = imageSize;
//}

void CameraEngine::SetParams_ZoomFactor( const Int& zoomFactor )
{
	if( !IsCameraDetected() )
		return;

	_camera->SetZoomFactorL( zoomFactor );
}

void CameraEngine::SetParams_DigitalZoomFactor( const Int& digitalZoomFactor )
{
	if( !IsCameraDetected() )
		return;

	_camera->SetDigitalZoomFactorL( digitalZoomFactor );
}

void CameraEngine::SetParams_Contrast( TInt contrast )
{
	if( !IsCameraDetected() )
		return;
	
	_camera->SetContrastL( contrast );
}

void CameraEngine::SetParams_Brightness( TInt brightness )
{
	if( !IsCameraDetected() )
		return;
	
	_camera->SetBrightnessL( brightness );
}

void CameraEngine::SetParams_Flash( CCamera::TFlash flashMode )
{
	if( !IsCameraDetected() )
		return;
	
	_camera->SetFlashL( flashMode );
}

void CameraEngine::SetParams_Exposure( CCamera::TExposure exposure )
{
	if( !IsCameraDetected() )
		return;
	
	_camera->SetExposureL( exposure );
}

void CameraEngine::SetParams_WhiteBalance( CCamera::TWhiteBalance whiteBalance )
{
	if( !IsCameraDetected() )
		return;
	
	_camera->SetWhiteBalanceL( whiteBalance );
}

void CameraEngine::SetParams_Portrait()
{
	if( !IsCameraDetected() )
		return;

	if( _isPortrait )
		return;
	
	StopViewFinder();
	
	_isPortrait = True;

	delete _imageManipulator;
	_imageManipulator = 0;
	
	StartViewFinder();
}

void CameraEngine::SetParams_Landscape()
{
	if( !IsCameraDetected() )
		return;

	if( !_isPortrait )
		return;
	
	_imageManipulator = new ImageManipulator();
	_imageManipulator->_manipulation = this;
	_imageManipulator->_manipulationDelegate = 
		ImageManipulator::Manipulation::Delegate( &CameraEngine::ManipulationComplete );
	
	StopViewFinder();
	_isPortrait = False;
	StartViewFinder();
}

//------------------------------------------------------
//	Getters
//------------------------------------------------------
//void CameraEngine::GetParams_ImageSize( TSize& imageSize )
//{
//	if( !IsCameraDetected() )
//		return;
//
//	imageSize = _imageSize;
//}

void CameraEngine::GetParams_ZoomFactor( Int& zoomFactor )
{
	if( !IsCameraDetected() )
		return;

	zoomFactor = _camera->ZoomFactor();
}

void CameraEngine::GetParams_ZoomFactorMin( Int& zoomFactorMin )
{
	if( !IsCameraDetected() )
		return;
	
	zoomFactorMin = _cameraInfo->iMinZoom;
}

void CameraEngine::GetParams_ZoomFactorMax( Int& zoomFactorMax )
{
	if( !IsCameraDetected() )
		return;
	
	zoomFactorMax = _cameraInfo->iMaxZoom;
}

void CameraEngine::GetParams_DigitalZoomFactor( Int& digitalZoomFactor )
{
	if( !IsCameraDetected() )
		return;

	digitalZoomFactor = _camera->DigitalZoomFactor();
}

void CameraEngine::GetParams_DigitalZoomFactorMin( Int& digitalZoomFactorMin )
{
	if( !IsCameraDetected() )
		return;
	
	digitalZoomFactorMin = 0;
}

void CameraEngine::GetParams_DigitalZoomFactorMax( Int& digitalZoomFactorMax )
{
	if( !IsCameraDetected() )
		return;
	
	digitalZoomFactorMax = _cameraInfo->iMaxDigitalZoom;
}

void CameraEngine::GetParams_Contrast( Int& contrast )
{
	if( !IsCameraDetected() )
		return;
	
	contrast = _camera->Contrast();
}

void CameraEngine::GetParams_ContrastMin( Int& contrastMin )
{
	if( !IsCameraDetected() )
		return;
	
	contrastMin = -100;
}

void CameraEngine::GetParams_ContrastMax( Int& contrastMax )
{
	if( !IsCameraDetected() )
		return;
	
	contrastMax = 100;
}

void CameraEngine::GetParams_Brightness( Int& brightness )
{
	if( !IsCameraDetected() )
		return;
	
	brightness = _camera->Brightness();
}

void CameraEngine::GetParams_BrightnessMin( Int& brightnessMin )
{
	if( !IsCameraDetected() )
		return;
	
	brightnessMin = -100;
}

void CameraEngine::GetParams_BrightnessMax( Int& brightnessMax )
{
	if( !IsCameraDetected() )
		return;
	
	brightnessMax = 100;
}

void CameraEngine::GetParams_Flash( Int& flash )
{
	if( !IsCameraDetected() )
		return;
	
	flash = _camera->Flash();
}

void CameraEngine::GetParams_FlashMin( Int& flash )
{
	if( !IsCameraDetected() )
		return;
	
	flash = CCamera::EFlashNone;
}

void CameraEngine::GetParams_FlashMax( Int& flash )
{
	if( !IsCameraDetected() )
		return;
	flash = CCamera::EFlashVideoLight;
}

void CameraEngine::GetParams_Exposure( Int& exposure )
{
	if( !IsCameraDetected() )
		return;
	
	exposure = _camera->Exposure();
}

void CameraEngine::GetParams_ExposureMin( Int& exposure )
{
	if( !IsCameraDetected() )
		return;
	
	exposure = CCamera::EExposureAuto;
}

void CameraEngine::GetParams_ExposureMax( Int& exposure )
{
	if( !IsCameraDetected() )
		return;
	
	exposure = CCamera::EExposureInfra;
}

void CameraEngine::GetParams_WhiteBalance( Int& whiteBalance )
{
	if( !IsCameraDetected() )
		return;

	whiteBalance = _camera->WhiteBalance();
}

void CameraEngine::GetParams_WhiteBalanceMin( Int& whiteBalance )
{
	if( !IsCameraDetected() )
		return;
	
	whiteBalance = CCamera::EWBAuto;
}

void CameraEngine::GetParams_WhiteBalanceMax( Int& whiteBalance )
{
	if( !IsCameraDetected() )
		return;
	
	whiteBalance = CCamera::EWBDaylightUnderWater;
}

//------------------------------------------------------
//	General manipulation
//------------------------------------------------------
void CameraEngine::StartViewFinder()
{
	if( !IsCameraDetected() )
		return;

	// request for camera usage
	// when ready the "ReserveComplete()" will be called
	_cameraState = eFinderInitializing;
	_camera->Reserve();
}

void CameraEngine::StartViewFinder( CCoeControl* control )
{
	_controlToDrawOn = control;
	StartViewFinder();
}

void CameraEngine::StopViewFinder()
{
	if( !IsCameraDetected() )
		return;
	
	if( _cameraState == eFinderActive )
	{
		_camera->PowerOff();
		_camera->Release();
		
	}
	
	_cameraState = eFinderInactive;
}

TBool CameraEngine::IsCameraDetected()
{
	return CCamera::CamerasAvailable();
}

const CameraEngine::CameraInfo& CameraEngine::GetCameraInfo()
{
	if( !_cameraInfo )
	{
		_cameraInfo = new CameraInfo();
//		memset( _cameraInfo, 0, sizeof(CameraInfo) );
	}

	if( IsCameraDetected() )
		_camera->CameraInfo( *_cameraInfo );
	
	return *_cameraInfo;
}

void CameraEngine::GetCameraInfoAsString( String& string )
{
	TRealFormat realFormat( 8, 5 );
	realFormat.iType = KRealFormatFixed | KDoNotUseTriads;
	
	string.Append( _L("HarwareVersion = ") );
	string.Append( _cameraInfo->iHardwareVersion.Name() );
	
	string.Append( _L("\n\n") );
	string.Append( _L("SoftwareVersion = ") );
	string.Append( _cameraInfo->iSoftwareVersion.Name() );
	
	string.Append( _L("\n\n") );
	string.Append( _L("CameraOrientation = ") );
	if( _cameraInfo->iOrientation == TCameraInfo::EOrientationOutwards )		string.Append( _L("Outwards") );
	else if( _cameraInfo->iOrientation == TCameraInfo::EOrientationInwards )	string.Append( _L("Inwards") );
	else if( _cameraInfo->iOrientation == TCameraInfo::EOrientationMobile )	string.Append( _L("Mobile") );
	else if( _cameraInfo->iOrientation == TCameraInfo::EOrientationUnknown )	string.Append( _L("Unknown") );

	string.Append( _L("\n\n") );
	string.Append( _L("OptionsSupported = ") );
	if( _cameraInfo->iOptionsSupported && TCameraInfo::EViewFinderDirectSupported )	string.Append( _L("ViewFinderDirect,") );
	if( _cameraInfo->iOptionsSupported && TCameraInfo::EViewFinderBitmapsSupported )	string.Append( _L("ViewFinderBitmaps,") );
	if( _cameraInfo->iOptionsSupported && TCameraInfo::EImageCaptureSupported )		string.Append( _L("ImageCapture,") );
	if( _cameraInfo->iOptionsSupported && TCameraInfo::EVideoCaptureSupported )		string.Append( _L("VideoCapture,") );
	if( _cameraInfo->iOptionsSupported && TCameraInfo::EViewFinderMirrorSupported )	string.Append( _L("ViewFinderMirror,") );
	if( _cameraInfo->iOptionsSupported && TCameraInfo::EContrastSupported )			string.Append( _L("Contrast,") );
	if( _cameraInfo->iOptionsSupported && TCameraInfo::EBrightnessSupported )			string.Append( _L("Brightness,") );
	if( _cameraInfo->iOptionsSupported && TCameraInfo::EViewFinderClippingSupported )	string.Append( _L("ViewFinderClipping,") );
	if( _cameraInfo->iOptionsSupported && TCameraInfo::EImageClippingSupported )		string.Append( _L("ImageClipping,") );
	if( _cameraInfo->iOptionsSupported && TCameraInfo::EVideoClippingSupported )		string.Append( _L("VideoClipping") );
	
	string.Append( _L("\n\n") );
	string.Append( _L("FlashModesSupported = ") );
	if( _cameraInfo->iFlashModesSupported && CCamera::EFlashNone )	string.Append( _L("FlashNone,") );
	else
	{
		if( _cameraInfo->iFlashModesSupported && CCamera::EFlashAuto )				string.Append( _L("Auto,")			);
		if( _cameraInfo->iFlashModesSupported && CCamera::EFlashForced )			string.Append( _L("Forced,")		);
		if( _cameraInfo->iFlashModesSupported && CCamera::EFlashFillIn )			string.Append( _L("FillIn,")		);
		if( _cameraInfo->iFlashModesSupported && CCamera::EFlashRedEyeReduce )		string.Append( _L("RedEyeReduce,")	);
		if( _cameraInfo->iFlashModesSupported && CCamera::EFlashSlowFrontSync )	string.Append( _L("SlowFrontSync,")	);
		if( _cameraInfo->iFlashModesSupported && CCamera::EFlashSlowRearSync )		string.Append( _L("SlowRearSync,")	);
		if( _cameraInfo->iFlashModesSupported && CCamera::EFlashManual )			string.Append( _L("Manual,")		);
		if( _cameraInfo->iFlashModesSupported && CCamera::EFlashVideoLight )		string.Append( _L("VideoLight")		);
		
	}
	
	string.Append( _L("\n\n") );
	string.Append( _L("ExposureModesSupported = ") );
	if( _cameraInfo->iExposureModesSupported && CCamera::EExposureAuto )				string.Append( _L("ExposureAuto,")				);
	if( _cameraInfo->iExposureModesSupported && CCamera::EExposureNight )				string.Append( _L("ExposureNight,")			);
	if( _cameraInfo->iExposureModesSupported && CCamera::EExposureBacklight )			string.Append( _L("ExposureBacklight,")		);
	if( _cameraInfo->iExposureModesSupported && CCamera::EExposureCenter )				string.Append( _L("ExposureCenter,")			);
	if( _cameraInfo->iExposureModesSupported && CCamera::EExposureSport )				string.Append( _L("ExposureSport,")			);
	if( _cameraInfo->iExposureModesSupported && CCamera::EExposureVeryLong )			string.Append( _L("ExposureVeryLong,")			);
	if( _cameraInfo->iExposureModesSupported && CCamera::EExposureSnow )				string.Append( _L("ExposureSnow,")				);
	if( _cameraInfo->iExposureModesSupported && CCamera::EExposureBeach )				string.Append( _L("ExposureBeach,")			);
	if( _cameraInfo->iExposureModesSupported && CCamera::EExposureProgram )			string.Append( _L("ExposureProgram,")			);
	if( _cameraInfo->iExposureModesSupported && CCamera::EExposureAperturePriority )	string.Append( _L("ExposureAperturePriority,")	);
	if( _cameraInfo->iExposureModesSupported && CCamera::EExposureManual )				string.Append( _L("ExposureManual,")			);
	if( _cameraInfo->iExposureModesSupported && CCamera::EExposureSuperNight )			string.Append( _L("ExposureSuperNight,")		);
	if( _cameraInfo->iExposureModesSupported && CCamera::EExposureInfra )				string.Append( _L("ExposureInfra")				);

	string.Append( _L("\n\n") );
	string.Append( _L("WhiteBalanceModesSupported = ") );
	if( _cameraInfo->iWhiteBalanceModesSupported && CCamera::EWBAuto )					string.Append( _L("EWBAuto,")				);
	if( _cameraInfo->iWhiteBalanceModesSupported && CCamera::EWBDaylight )				string.Append( _L("EWBDaylight,")			);
	if( _cameraInfo->iWhiteBalanceModesSupported && CCamera::EWBCloudy )				string.Append( _L("EWBCloudy,")				);
	if( _cameraInfo->iWhiteBalanceModesSupported && CCamera::EWBTungsten )				string.Append( _L("EWBTungsten,")			);
	if( _cameraInfo->iWhiteBalanceModesSupported && CCamera::EWBFluorescent )			string.Append( _L("EWBFluorescent,")		);
	if( _cameraInfo->iWhiteBalanceModesSupported && CCamera::EWBFlash )				string.Append( _L("EWBFlash,")				);
	if( _cameraInfo->iWhiteBalanceModesSupported && CCamera::EWBSnow )					string.Append( _L("EWBSnow,")				);
	if( _cameraInfo->iWhiteBalanceModesSupported && CCamera::EWBBeach )				string.Append( _L("EWBBeach,")				);
	if( _cameraInfo->iWhiteBalanceModesSupported && CCamera::EWBManual )				string.Append( _L("EWBManual,")				);
	if( _cameraInfo->iWhiteBalanceModesSupported && CCamera::EWBShade )				string.Append( _L("EWBShade,")				);
	if( _cameraInfo->iWhiteBalanceModesSupported && CCamera::EWBAutoSkin )				string.Append( _L("EWBAutoSkin,")			);
	if( _cameraInfo->iWhiteBalanceModesSupported && CCamera::EWBHorizon )				string.Append( _L("EWBHorizon,")			);
	if( _cameraInfo->iWhiteBalanceModesSupported && CCamera::EWBDaylightUnderWater )	string.Append( _L("EWBDaylightUnderWater")	);

	string.Append( _L("\n\n") );
	string.Append( _L("MinMaxZoom = ") );
	string.AppendNum( _cameraInfo->iMinZoom ); string.Append( _L(",") );  
	string.AppendNum( _cameraInfo->iMaxZoom );

	string.Append( _L("\n\n") );
	string.Append( _L("MaxDigitalZoom = ") );string.AppendNum( _cameraInfo->iMaxDigitalZoom );

	string.Append( _L("\n\n") );
	string.Append( _L("MinMaxZoomFactor = ") );
	string.AppendNum( _cameraInfo->iMinZoomFactor, realFormat ); string.Append( _L(",") );
	string.AppendNum( _cameraInfo->iMaxZoomFactor, realFormat );

	string.Append( _L("\n\n") );
	string.Append( _L("MaxDigitalZoomFactor = ") ); string.AppendNum( _cameraInfo->iMaxDigitalZoomFactor, realFormat );

	string.Append( _L("\n\n") );
	string.Append( _L("NumImageSizesSupported = ") ); string.AppendNum( _cameraInfo->iNumImageSizesSupported );
	
	string.Append( _L("\n\n") );
	string.Append( _L("ImageFormatsSupported = ") ); string.AppendNum( _cameraInfo->iImageFormatsSupported );

	string.Append( _L("\n\n") );
	string.Append( _L("NumVideoFrameSizesSupported = ") ); string.AppendNum( _cameraInfo->iNumVideoFrameSizesSupported );

	string.Append( _L("\n\n") );
	string.Append( _L("NumVideoFrameRatesSupported = ") ); string.AppendNum( _cameraInfo->iNumVideoFrameRatesSupported );

	string.Append( _L("\n\n") );
	string.Append( _L("VideoFrameFormatsSupported = ") ); string.AppendNum( _cameraInfo->iVideoFrameFormatsSupported );

	string.Append( _L("\n\n") );
	string.Append( _L("MaxFramesPerBufferSupported = ") ); string.AppendNum( _cameraInfo->iMaxFramesPerBufferSupported );
	
	string.Append( _L("\n\n") );
	string.Append( _L("MaxBuffersSupported = ") ); string.AppendNum( _cameraInfo->iMaxBuffersSupported );
}

void CameraEngine::ReserveComplete( TInt aError )
{
	// power the camera on if we're to be using the camera
	// when powered on the "PowerOnComplete()" will be called 
	if( aError == KErrNone )		
		_camera->PowerOn();
	else
		_cameraState = eFinderFailed;
}

void CameraEngine::PowerOnComplete( TInt aError )
{
	CCamera::TFormat imageFormat;
	
	if( _cameraInfo->iImageFormatsSupported & CCamera::EFormatFbsBitmapColor16M )
		imageFormat = CCamera::EFormatFbsBitmapColor16M;
	
    else if( _cameraInfo->iImageFormatsSupported & CCamera::EFormatFbsBitmapColor64K )
        imageFormat = CCamera::EFormatFbsBitmapColor64K;
	
    else
        imageFormat = CCamera::EFormatFbsBitmapColor4K;

	if( aError == KErrNone )
	{
		// try EXIF JPEG format
//		TRAPD( exifError, _camera->PrepareImageCaptureL(CCamera::EFormatExif,1) );
//		if( exifError )
		_camera->PrepareImageCaptureL( imageFormat, 1 );

		TRect imageRect = _controlToDrawOn->Rect();
		
		Int width = imageRect.iBr.iX - imageRect.iTl.iX;
		Int height = imageRect.iBr.iY - imageRect.iTl.iY;
		if( !_isPortrait )
		{
			width = imageRect.iBr.iY - imageRect.iTl.iY;
			height = imageRect.iBr.iX - imageRect.iTl.iX;
		}
		
		TSize imageSize;
		imageSize.SetSize( width, height );
		TRAPD( ignored, _camera->StartViewFinderBitmapsL(imageSize) );

		_cameraState = eFinderActive;
	}
	else
	{
		_camera->Release();
		_cameraState = eFinderFailed;
	}
}

void CameraEngine::ViewFinderFrameReady( CFbsBitmap& bitmap )
{
	// Keep backlight on while finder is active
    User::ResetInactivityTime();

    _currentViewFinderImage.Duplicate( bitmap.Handle() );

//    if( _isPortrait )
    	DrawImageOnControl();
//    else
//    	_imageManipulator->Rotate( _currentViewFinderImage, CBitmapRotator::ERotation90DegreesClockwise );    	
}

void CameraEngine::ImageReady( CFbsBitmap* bitmap, HBufC8* data, TInt error )
{
	if( _imageShot && _imageShotDelegate )
		(_imageShot->*_imageShotDelegate)( bitmap );
	
	StartViewFinder();
}

void CameraEngine::FrameBufferReady( MFrameBuffer* frameBuffer, TInt error )
{
	if( _videoFrame && _videoFrameDelegate )
		(_videoFrame->*_videoFrameDelegate)( frameBuffer );
}

void CameraEngine::ManipulationComplete( ImageManipulator::ImageOperation imageOperation )
{
	if( ImageManipulator::eSave == imageOperation )
		return;
	
    // we're don rotating, draw it
	DrawImageOnControl();
}

void CameraEngine::DrawImageOnControl()
{
    if( !_controlToDrawOn )
    	return;
    
//    TSize realImageSize = _currentViewFinderImage.SizeInPixels();
    
	TPoint position = _controlToDrawOn->Rect().iTl;
	
    CWindowGc& graphicsContext = _controlToDrawOn->SystemGc();
    graphicsContext.Activate( *_controlToDrawOn->DrawableWindow() );
    graphicsContext.BitBlt( position, &_currentViewFinderImage );
    graphicsContext.Deactivate();
    
}
