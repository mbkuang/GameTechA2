#ifndef __Wall_h_
#define __Wall_h_

#include <GameObject.h>

class Wall : public GameObject {
private:
	Ogre::Vector3 position;
	Ogre::Vector3 scale;
	btVector3 setting;
	Ogre::String material;
	float lastTime;

public:
    Wall(Ogre::String newName, Ogre::SceneManager* scnMgr, Simulator* sim, 
    	float xPosition = 0, float yPosition = 0, float zPosition = 0,
    	float xScale = 1, float yScale = 1, float zScale = 1,
    	Ogre::String material = "");  
    ~Wall();
    void update(float elapsedTime);
};


#endif