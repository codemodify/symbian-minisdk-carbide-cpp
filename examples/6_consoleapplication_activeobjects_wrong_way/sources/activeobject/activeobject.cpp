
// local headers
#include "activeobject.h"
#include "../commonfunctionalities/commonfunctionalities.h"

ActiveObject::ActiveObject() :
	CActive( EPriorityStandard ),
	_theCallback( 0 )
{
//	CActiveScheduler::Add( this );
}

ActiveObject::~ActiveObject()
{
	Cancel();
}

void ActiveObject::setCallback( TheCallback theCallback, TAny* parameter )
{
	_theCallback = theCallback;
	_parameter = parameter;
	
	emulateOutstandingRequest();
}

void ActiveObject::DoCancel()
{
	// FIXME: add corresponding code here
	// ...
	
	// here pretend to do some cancelation for now
}

void ActiveObject::RunL()
{
	// if an error occured, handle it in "RunError()"
	User::LeaveIfError( iStatus.Int() );

	// now handle the result here
	if( 0 != _theCallback )
		_theCallback( _parameter );
}

void ActiveObject::emulateOutstandingRequest()
{
	// make a new asychronous request here
	// NOTE:in case we don't actually do any of the outstanding requests
	//		we need to emulate it.
	if( IsActive() )
	{
		// this is a programming error, so a PANIC is ok here
		User::Panic( Text("ActiveObject"), KErrInUse );
	}
	else
	{
		TRequestStatus* requestStatus = &iStatus;
		User::RequestComplete( requestStatus, KErrNone );
		
		// mark this object active
//		SetActive();	
	}	
}

//---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
//	Implement here the 'thread' that will issue "CActiveScheduler::Stop()"
//	if there are no Active Objects running.
//---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
TInt checkForAnyActiveObjects( TAny* data )
{
	ActiveObjectList* activeObjects = (ActiveObjectList*) data;
	if( NULL == activeObjects )
		return 0;

	TSglQueIter<ActiveObject> queueIterator( *activeObjects );
	queueIterator.SetToFirst();
	while( NULL != queueIterator )
	{
		ActiveObject& activeObject = *queueIterator;
		if( activeObject.IsActive() )
			return 0;

		queueIterator++;
	}		
		
	CActiveScheduler::Stop();
	
	return 0;
}
