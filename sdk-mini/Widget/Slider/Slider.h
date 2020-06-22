#ifndef __Slider_H__
#define __Slider_H__

// local headers
#include <sdk-mini/Widget/Widget.h>

class Slider :	public Widget,
				public Subscriber< TKeyEvent& >,	// subscribe for key events from UI
				public SubscribersInformer< const Int& >	// emit current value when changed
{
    public:
    	Slider();
        virtual ~Slider();

        virtual void SetMinValue( Int value );
        virtual void SetValue( Int value );
        virtual void SetMaxValue( Int value );
        
        virtual const Int& GetValue();

	// handlers for the subscriptions 
    public:
        void KeyPress( TKeyEvent& keyEvent );

    // From CoeControl
    protected:
    	virtual void Draw( const TRect& aRect ) const;

    // interfanll stuff
    protected:
    	Int	_minValue;
    	Int	_value;
    	Int	_maxValue;
};

#endif
