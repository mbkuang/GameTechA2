#ifndef __Frog_h_
#define __Frog_h_

#include "GameObject.h"
#include "Node.h"
#include "AIManager.h"
#include "Shooter.h"
#include <math.h>       /* round */

class Frog : public GameObject {
    protected:
        Ogre::Vector3 position;
        float radius;
        Ogre::String material;
        AIManager* aiMgr;
        float timeSinceLastJump;
        float lastContactTime;

    public:
        Frog(Ogre::String name, Ogre::SceneManager* sceneMgr, Simulator* simulator,
    	Ogre::Vector3 position, float radius, Ogre::String material, AIManager* aiMgr);

        ~Frog();

        void jump();

        void update(float elapsedTime);
        
};

#endif
