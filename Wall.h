#include <GameObject.h>

class Wall : public GameObject {
public:
    Wall(Ogre::SceneManager* scnMgr);
    ~Wall();
};
