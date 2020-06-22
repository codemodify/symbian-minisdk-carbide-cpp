
#include <eikenv.h>

#include "Widget.h"

Widget::Widget() :
	_isFocused( False )
//:	backgroundBitmap( 0 )
{
	InitComponentArrayL() ;

	CreateWindowL();
	ActivateL();
}

Widget::~Widget()
{
	// NOTE: we don't have to destroy them, it will be done once we called the "InitComponentArrayL()"
	
	// destroy all the child widgets
	//	for( TInt childIndex=0; childIndex < _children.Count(); childIndex++ )
	//		delete _children[ childIndex ];

	_children.Reset();
}

void Widget::AddChildWidget( Widget* childWidget )
{
	// keep track of the chidren since the trash below is just for the framework 
	// pretending to pretend to have flexibility
	_children.Append( childWidget );

	// add it here to make super framework happy, 
	// but don't use it much because of its pseudo flexibility
	Components().AppendLC( childWidget );
	CleanupStack::Pop( childWidget ) ; // the fucker above leaves some trash to collect
	
	// layout content
	LayoutChildren();
}

void Widget::SetRect( const TRect& rect )
{
	_rect = rect;
	
	// let the framework think it has the control
	CCoeControl::SetRect( rect );
}

void Widget::SetFocus( Bool isFocused )
{
	_isFocused = isFocused;
	
	// let the framework think it has the control
	CCoeControl::SetFocus( isFocused, EDrawNow );
}

Bool Widget::IsFocused()
{
	return _isFocused;
}

void Widget::FocusWidget( Widget* childWidget )
{
	Int cc =_children.Count();
	
	for( Int childIndex=0; childIndex < cc; childIndex++ )
		_children[ childIndex ]->SetFocus( _children[childIndex]==childWidget );
	
	LayoutChildren();
}

void Widget::FocusNextWidget()
{
	for( Int childIndex=0; childIndex < _children.Count(); childIndex++ )
	{
		Bool isFocused = _children[childIndex]->IsFocused();
		if( isFocused && ((childIndex+1) < _children.Count()) )
		{
			FocusWidget( _children[childIndex+1] );
			return;
		}
	}
}

void Widget::FocusPreviousWidget()
{
	for( Int childIndex=0; childIndex < _children.Count(); childIndex++ )
	{
		Bool isFocused = _children[childIndex]->IsFocused(); 
		if( isFocused && ((childIndex-1) >= 0) )
		{
			FocusWidget( _children[childIndex-1] );
			return;
		}
	}
}

Widget* Widget::GetFocusedWidget()
{
	for( Int childIndex=0; childIndex < _children.Count(); childIndex++ )
		if( _children[childIndex]->IsFocused() )
			return _children[childIndex];
	
	return 0;	
}

void Widget::LayoutChildren()
{
	// the idea of layouting is to have a stack of widgets visible
	// max-height is 40.

	const Int		childrenCount = _children.Count();
	const Int		maxChildHeight = 40;
	const TRect&	parentRect = _rect;

	TRect childRect = parentRect;
	for( Int childIndex=0; childIndex < _children.Count(); childIndex++ )
	{
		childRect.iTl.iY = parentRect.iBr.iY - ((childIndex+1) * maxChildHeight);
		childRect.iBr.iY = childRect.iTl.iY + maxChildHeight;
		_children[ childIndex ]->SetRect( childRect );
		_children[ childIndex ]->DrawNow();
	}
}

void Widget::SizeChanged()
{
	LayoutChildren();
}

TInt Widget::CountComponentControls() const
{
	Int childrenCount = _children.Count();

	return childrenCount;
}

CCoeControl* Widget::ComponentControl( TInt aIndex ) const
{
	Int childrenCount = _children.Count();
	if( (aIndex<childrenCount) && (aIndex>=0) )
		return _children[ aIndex ];

	return 0;
}

void Widget::Draw( const TRect& boundariesRect ) const
{
	// what a basic widget is drawing is the background
	
    CWindowGc &graphicContext = SystemGc();

    graphicContext.SetBrushStyle( CGraphicsContext::ESolidBrush );
    graphicContext.SetBrushColor( KRgbBlack );
    graphicContext.DrawRect( boundariesRect );
}
