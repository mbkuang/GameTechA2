#include <GameObject.h>

class Paddle : public GameObject {
public:
    Paddle(Ogre::String newName, Ogre::SceneManager* scnMgr, Simulator* sim);
    ~Paddle();
    void move(Ogre::Real x, Ogre::Real y, Ogre::Real z,
        float xMin, float xMax, float yMin, float yMax);
};
