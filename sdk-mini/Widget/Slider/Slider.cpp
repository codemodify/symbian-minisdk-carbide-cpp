
// system headers
#include <aknutils.h>
//#include <gulfont.h>
//#include <avkon.hrh> //Font ids

// local headers
#include "Slider.h"

Slider::Slider() :
	Widget(),
	_minValue( 0 ),
	_value( 0 ),
	_maxValue( 0 )
{}

Slider::~Slider()
{}

void Slider::SetMinValue( Int value )
{
	_minValue = value;
}

void Slider::SetValue( Int value )
{
	if( (value<_minValue) || (value>_maxValue) )
		return;
	
	_value = value;

	DrawNow();
	
    InformSubscribers( GetValue() );
}

void Slider::SetMaxValue( Int value )
{
	_maxValue = value;
}

const Int& Slider::GetValue()
{
	return _value;
}

void Slider::KeyPress( TKeyEvent& keyEvent )
{
	if( !_isFocused )
		return;
	
    switch( keyEvent.iCode )
	{
        case EKeyUpArrow:
        case EKeyLeftArrow:
        	SetValue( GetValue() + 1 );
        	break;

        case EKeyDownArrow:
        case EKeyRightArrow:
    		SetValue( GetValue() - 1 );
        	break;

        default:
            break;
    }
}

void Slider::Draw( const TRect& boundariesRect ) const
{
	CWindowGc &graphicContext = SystemGc();
	
	// draw background
	Widget::Draw( boundariesRect ); // the parameter is ignored any way

	// draw outline	
	const Int nonFocusedOutlineWidth = 1;
	const Int focusedOutlineWidth = 3;
	const Int outlineWidth = _isFocused ? focusedOutlineWidth : nonFocusedOutlineWidth;

	Point topLeft = boundariesRect.iTl;
	topLeft.iX += 1; // some space around the control
	topLeft.iY += 1; // some space around the control
	
	Point bottomRight = boundariesRect.iBr;
	bottomRight.iX -= 1; // some space around the control
	bottomRight.iY -= 1; // some space around the control

	TSize penSize( outlineWidth, outlineWidth );
	graphicContext.SetBrushStyle( CWindowGc::ENullBrush );
	graphicContext.SetPenSize( penSize );
	graphicContext.SetPenColor( KRgbGreen );
	
	TRect fixedRect( topLeft, bottomRight );
	graphicContext.DrawRect( fixedRect );


	// prepare for text drawing
	String valueAsString;
	graphicContext.SetPenColor( KRgbGreen );
	
	const Int ourHeight = boundariesRect.iBr.iY - boundariesRect.iTl.iY;
	const Int ourWidth  = boundariesRect.iBr.iX - boundariesRect.iTl.iX;
	const Int borderSpaceForText = 5;

	
	
	// draw min value
	const CFont* minMaxFont = CEikonEnv::Static()->TitleFont();
	graphicContext.UseFont( minMaxFont );	

	IntToString( _minValue, valueAsString );
	graphicContext.DrawText( valueAsString, fixedRect, ourHeight/2 + minMaxFont->HeightInPixels()/2, CGraphicsContext::ELeft, borderSpaceForText );

	
	
	// draw the value
	const CFont* valueFont = LatinPlain12();
	graphicContext.UseFont( valueFont );
	
	IntToString( _value, valueAsString );
	graphicContext.DrawText( valueAsString, fixedRect, ourHeight/2 + valueFont->HeightInPixels()/2 , CGraphicsContext::ECenter );

	
	
	// draw the rectangle and the current position within the bar
	Point barPoint1;	barPoint1.SetXY( topLeft.iX, topLeft.iY );
	Point barPoint2;	barPoint2.SetXY( bottomRight.iX, bottomRight.iY );
	
	TRect barRect( barPoint1, barPoint2 );
	barRect.iTl.iX += 5*borderSpaceForText;
	barRect.iTl.iY += borderSpaceForText;
	barRect.iBr.iX -= 5*borderSpaceForText;
	barRect.iBr.iY -= ourHeight/2 + borderSpaceForText;
	
	TSize barPenSize( nonFocusedOutlineWidth, nonFocusedOutlineWidth );
	graphicContext.SetPenSize( barPenSize );
	graphicContext.DrawRect( barRect );
	
	Int currentPercetValue = _maxValue ? ((_value*100) / _maxValue) : 0;
	Int barWidth = barRect.iBr.iX - barRect.iTl.iX;
	Int percentPositionWithinTheBar = (barWidth * currentPercetValue) / 100;
	
	Point linePoint1;
	linePoint1.SetXY( barRect.iTl.iX + percentPositionWithinTheBar, barRect.iTl.iY  );

	Point linePoint2;
	linePoint2.SetXY( barRect.iTl.iX + percentPositionWithinTheBar, barRect.iBr.iY  );

	graphicContext.DrawLine( linePoint1, linePoint2 );

	
	
	// draw the end label
	graphicContext.UseFont( minMaxFont );
	
	IntToString( _maxValue, valueAsString );
	graphicContext.DrawText( valueAsString, fixedRect, ourHeight/2 + minMaxFont->HeightInPixels()/2, CGraphicsContext::ERight, borderSpaceForText );
}
