#ifndef active_object_h
#define active_object_h

// Symbian headers
#include <e32base.h> // for: CActive

/*****************************************************************************
* 
*	This class is designed to facilitate instantiation for an Active Object
* 
*****************************************************************************/ 
class ActiveObject : public CActive
{
	public:
		ActiveObject();
		~ActiveObject();
		
	// interfaces to use
	public:
		typedef void (*TheCallback)(TAny*);
		void setCallback( TheCallback theCallback, TAny* parameter );
		
		TSglQueLink iLink;

	// we need to override these to become an Active Object
	protected:
		void DoCancel();
		void RunL();

	private:
		void emulateOutstandingRequest();
		
	// internal stuff
	private:
		TheCallback _theCallback;
		TAny* _parameter;
};


typedef TSglQue<ActiveObject> ActiveObjectList;
TInt checkForAnyActiveObjects( TAny* data );

#endif
