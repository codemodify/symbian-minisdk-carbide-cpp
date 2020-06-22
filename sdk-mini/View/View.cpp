
#include <coemain.h>
#include <coeaui.h>
#include <AknAppUi.h>

#include <sdk-mini/Widget/Widget.h> 
#include "View.h"

View::View( TInt resourceId, TUid viewId ) :
	_viewId( viewId ),
	_viewContent( 0 )
{
	// what is base construct meant to dod ?
	// - construct the base of who or what ? a military one ?
	BaseConstructL( resourceId );
}

View::~View()
{
	DestroyViewContent();
}

void View::SetViewContent( Widget* viewContent )
{
	_viewContent = viewContent;
	_viewContent->SetRect( ClientRect() );

	// we need to take into account some artificial mafia-style and formal-only framework
	// if you do not respect the forced and useless stack you can't move.
	CCoeEnv::Static()->AppUi()->AddToStackL( _viewContent );
}

Widget* View::GetViewContent()
{
	return _viewContent;
}

void View::DestroyViewContent()
{
    if( _viewContent )
    {
    	// once more about the informing the head of mafia about our intentions.
    	CCoeEnv::Static()->AppUi()->RemoveFromViewStack( *this, _viewContent );

        delete _viewContent;
        _viewContent = 0;
    }
}

TUid View::Id() const
{
    return _viewId;
}

void View::HandleClientRectChange()
{
    if( _viewContent )
    	_viewContent->SetRect( ClientRect() );
}

void View::DoDeactivate()
{
	DestroyViewContent();
}
