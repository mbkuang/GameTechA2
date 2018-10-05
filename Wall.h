#include <GameObject.h>

class Wall : public GameObject {
private:
	float xPosition;
	float yPosition;
	float zPosition;
	float xScale;
	float yScale;
	float zScale;

public:
    Wall(Ogre::String newName, Simulator* sim, Ogre::SceneManager* scnMgr, float xPosition = 0, float yPosition = 0, float zPosition = 0, float xScale = 1, float yScale = 1, float zScale = 1);
    ~Wall();
};
