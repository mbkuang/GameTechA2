#include <GameObject.h>

class Ball : public GameObject {
protected:
    float x, y, z;
public:
    Ball(Ogre::String newName, Simulator* sim, Ogre::SceneManager* scnMgr);
    ~Ball();
    
};
