#ifndef __Ball_h_
#define __Ball_h_

#include <GameObject.h>

class Ball : public GameObject {
protected:
    float x, y, z;
    float radius;
	Ogre::SceneNode* markerNode;
	Ogre::Entity* marker;

public:
    Ball(Ogre::String name, Ogre::SceneManager* sceneMgr, Simulator* simulator);
    ~Ball();
    Ogre::SceneNode* getMarkerNode();
};

#endif
