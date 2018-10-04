#include <GameObject.h>

class Wall : public GameObject {
public:
    Wall(Simulator* sim, Ogre::SceneManager* scnMgr);
    ~Wall();
};
