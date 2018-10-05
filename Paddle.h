#include <GameObject.h>

class Paddle : public GameObject {
public:
    Paddle(Ogre::String newName, Ogre::SceneManager* scnMgr, Simulator* sim);
    ~Paddle();
};
