#ifndef __Controller_h_
#define __Controller_h_

#include "BaseApplication.h"

public Controller : puble BaseApplication
{
public:
	Controller(void);
    virtual ~Controller(void);

protected:
	virtual void createFrameListener(void);
	virtual bool keyPressed(const OIS::KeyEvent &arg);
    virtual bool keyReleased(const OIS::KeyEvent &arg);
    virtual bool mouseMoved(const OIS::MouseEvent &arg);
    virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
}

#endif