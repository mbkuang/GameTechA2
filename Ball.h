#include <GameObject.h>

class Ball : public GameObject {
protected:
    float x, y, z;
public:
    Ball(Ogre::String name, Ogre::SceneManager* sceneMgr, Simulator* simulator);
    ~Ball();
    
};
