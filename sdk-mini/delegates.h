#ifndef __Delegates_H__
#define __Delegates_H__

// system headers
#include <e32std.h>

/*******************************************************************************
*
*	Utility functions to aid the below delegating
*	Usage:
*
*		BindEvent( source, prototype, destination, slot );
*
******************************************************************************/
#define BindEvent( eventGenerator, ParameterType, eventHandler, eventHandlerMethod ) \
	eventGenerator->SetSubscriber( eventHandler, Subscriber<ParameterType>::Delegate(&eventHandlerMethod) )



/*******************************************************************************
*
*	Defines a simple delagating approach.
*	Usage:
* 
*	class MyClass :	public TSomething, 
* 					public MSomething, 
* 					public Subscriber<CFbsBitmap&>		// subscribe for events with this type
*	{
*		MyClassEeventGenerator* _eventGenerator;
* 
* 		MyClass()
*		{
*			eventGenerator = new MyClassEeventGenerator();
* 
* 			// Usage 1
* 			eventGenerator->SetSubscriber( this, 
* 						Subscriber<CFbsBitmap&>::Delegate(&MyClassEeventGenerator::NewImageReady) );
* 
*			// Usage 2 - Trolls' style.
* 			// Read the below as:
*  			// when "eventGenerator" has an event(internal handler) with type(prototype) "CFbsBitmap"
*  			// it must inform us("this") by calling our method "NewImageReady()".
*
* 			BindEvent( eventGenerator, CFbsBitmap&, this, MyClassEeventGenerator::NewImageReady );
*		}
* 		
* 		void NnewImageReady( CFbsBitmap& bitmap ){  _do_here_something_wit_the_new_image_  }
* 	}
*
* 
*
*	class MyClassEeventGenerator :	public AgainTSomethingAndMSomething, 
* 									public MCameraObserver, 
* 									public SubscribersInformer<CFbsBitmap&>
*	{
* 		...
*		// This is comming from MCameraObserver
* 		void ViewFinderFrameReady( CFbsBitmap& aFrame )
*		{
* 			// Keep backlight on while finder is active
*			User::ResetInactivityTime();
*			InformSubscribers( aFrame );
*		}	
* 	}
*
*
******************************************************************************/ 



template< class ParameterType >
class Subscriber
{
	// It is important that this class is not defined esle where
	
	public:
//		typedef Subscriber< ParameterType >* Subscriberr;
		typedef void (Subscriber< ParameterType >::*Delegate)( ParameterType );
};

template< class ParameterType >
class SubscribersInformer
{
	// it is important that this class is not defined esle where
	
	// We need to redefine the types here locally because of Symbian's C++.
	// We can't use the types defined in previous class, for some reason it won't compile it for ARM
	private:
		typedef Subscriber< ParameterType >* TheSubscriber;
		typedef void (Subscriber< ParameterType >::*TheDelegate)( ParameterType );

	public:
		virtual ~SubscribersInformer()
		{
			_subcribers.Reset();
			_delegates.Reset();
		}

		//void SetSubscriber(	Subscriber< ParameterType >::Subscriberr subscriber,
		//						Subscriber< ParameterType >::Delegate delegate )
		void SetSubscriber(	TheSubscriber subscriber, TheDelegate delegate )
		{
			_subcribers.Append( subscriber );
			_delegates.Append( delegate );
		}

		void InformSubscribers( ParameterType parameter )
		{
			for( Int subscriberIndex=0; subscriberIndex < _subcribers.Count(); subscriberIndex++ )
			{
				TheSubscriber	subscriber	= _subcribers[ subscriberIndex ];
				TheDelegate		delegate	= _delegates[ subscriberIndex ];
				
				if( subscriber && delegate )
					(subscriber->*delegate)( parameter );
			}
		}

	private:
		RArray< TheSubscriber >	_subcribers;
		RArray< TheDelegate >	_delegates;
};

#endif


/*******************************************************************************
*
*	Why was choosen this type of delegating:
*
*	- Because you can attach subscription feature to the classes you want.
*	There is no need to have a basic virtual method to be implemented in
*	derived classes.
*
*	- Adds a very flexibel way to selectively pass data between anything.
*
******************************************************************************/
