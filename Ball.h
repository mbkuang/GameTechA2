#include <GameObject.h>

class Ball : public GameObject {
protected:
    Ogre::Real bRadius;
    Ogre::Vector3 bDirection;
    Ogre::Real bSpeed;
public:
    Ball(Ogre::String name, Ogre::SceneManager* sceneMgr, Simulator* simulator);
    ~Ball();
    Ogre::Vector3 getPosition();
    Ogre::Real getRadius();
};
