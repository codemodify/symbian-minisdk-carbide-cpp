#ifndef __View_H__
#define __View_H__

#include <aknview.h>

class Widget;

class View : public CAknView
{
	public:
		View( TInt resourceId, TUid viewId );
		virtual ~View();

		void SetViewContent( Widget* viewContent );
		Widget* GetViewContent();

	// from CAknView
    public:
        TUid Id() const;
        void HandleClientRectChange();

    protected:
        virtual void DoDeactivate();

        void DestroyViewContent();

    private:
    	TUid	_viewId;
        Widget*	_viewContent;

};

#endif
