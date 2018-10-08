#ifndef __OgreMotionState_h_
#define __OgreMotionState_h_

#include <btBulletDynamicsCommon.h>
#include <Ogre.h>

class OgreMotionState : public btMotionState {
protected:
	Ogre::SceneNode* mVisibleobj;
	btTransform mPos1;

public:
	OgreMotionState(const btTransform& initialpos, Ogre::SceneNode* node);
	virtual ~OgreMotionState();
	void setNode(Ogre::SceneNode* node);
	void updateTransform(btTransform& trans);
	virtual void getWorldTransform(btTransform& worldTrans) const;
	virtual void setWorldTransform(const btTransform& worldTrans);
};

#endif
