#include <GameObject.h>

class Ball : public GameObject {
protected:
    Ogre::Real bRadius;
    Ogre::Vector3 bDirection;
    Ogre::Real bSpeed;
public:
    Ball(Ogre::SceneManager* scnMgr);
    ~Ball();
    Ogre::Vector3 getPosition();
    Ogre::Real getRadius();
};
