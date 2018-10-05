#include <GameObject.h>

class Paddle : public GameObject {
protected:
    float x, y, z;
public:
    Paddle(Ogre::String newName, Ogre::SceneManager* scnMgr, Simulator* sim);
    ~Paddle();
};
