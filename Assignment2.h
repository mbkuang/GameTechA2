/*
-----------------------------------------------------------------------------
Filename:    Assignment2.h
-----------------------------------------------------------------------------

This source file is part of the
   ___                 __    __ _ _    _
  /___\__ _ _ __ ___  / / /\ \ (_) | _(_)
 //  // _` | '__/ _ \ \ \/  \/ / | |/ / |
/ \_// (_| | | |  __/  \  /\  /| |   <| |
\___/ \__, |_|  \___|   \/  \/ |_|_|\_\_|
      |___/
Tutorial Framework (for Ogre 1.9)
http://www.ogre3d.org/wiki/
-----------------------------------------------------------------------------
*/

#ifndef __Assignment2_h_
#define __Assignment2_h_

#include "BaseApplication.h"
#include <btBulletDynamicsCommon.h>
#include "Simulator.h"

//---------------------------------------------------------------------------

class Assignment2 : public BaseApplication
{
public:
    Assignment2(void);
    virtual ~Assignment2(void);

protected:
    virtual void createScene(void);
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& fe);

    virtual bool keyPressed(const OIS::KeyEvent &arg);
    virtual bool keyReleased(const OIS::KeyEvent &arg);

    virtual bool mouseMoved(const OIS::MouseEvent &arg);
    virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
};

//---------------------------------------------------------------------------

#endif // #ifndef __Assignment2_h_

//---------------------------------------------------------------------------
