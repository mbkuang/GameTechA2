#include <GameObject.h>

class Paddle : public GameObject {
protected:
    float x, y, z;
public:
    Paddle(Ogre::String newName, Simulator* sim, Ogre::SceneManager* scnMgr);
    ~Paddle();
};
