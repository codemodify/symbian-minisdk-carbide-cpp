#ifndef __Widget_H__
#define __Widget_H__

// system headers
#include <coecntrl.h>
#include <aknviewappui.h>

// local headers
#include <sdk-mini/types.h>

class Widget : public CCoeControl
{
    public:
        Widget();
        virtual ~Widget();

        void AddChildWidget( Widget* childWidget );
        
        void SetRect( const TRect& rect );
        
        void SetFocus( Bool isFocused = True );
        Bool IsFocused();
        
        void FocusWidget( Widget* childWidget );
        void FocusNextWidget();
        void FocusPreviousWidget();
        Widget* GetFocusedWidget();

    protected:
    	// acts as a layout manager for the children
        void LayoutChildren();


    // From CCoeControl
	protected:
		virtual void SizeChanged();
        virtual TInt CountComponentControls() const;
        virtual CCoeControl* ComponentControl( TInt index ) const;
        virtual void Draw( const TRect& aRect ) const;


	protected:
		Bool _isFocused;
		
		// we're going to need this since the passed rect from the view is getting lost 
		TRect	_rect;
		
		// we need this array since the Components() is useless as in traditional way of symbian
		RPointerArray<Widget>	_children;

};

#endif
