#ifndef __Simulator_h_
#define __Simulator_h_

#include <Ogre.h>
#include <btBulletDynamicsCommon.h>
#include "GameObject.h"
#include "Player.h"
#include "Sound.h"
#include "Overlay.h"

class GameObject;
class Player;
class Overlay;

class Simulator {
protected:
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btBroadphaseInterface* overlappingPairCache;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;
    //std::vector<btCollisionShape*> collisionShapes;
	btAlignedObjectArray<btCollisionShape*> collisionShapes;
	std::map<Ogre::String, GameObject*> objMap;
	std::deque<GameObject*> objList;
	std::map<Ogre::String, Player*> players;
	bool isPaused;

public:
	Simulator();
	~Simulator();
	void addObject(GameObject* o);
	bool removeObject(GameObject* o);
	void stepSimulation(const Ogre::Real elapsedTime, int maxSubSteps = 1, const Ogre::Real fixedTimestep = 1.0f/60.0f);
    btDiscreteDynamicsWorld* getDynamicsWorld();
    //std::vector<btCollisionShape*> getCollisionShapes();
    //int getCollisionObjectCount();
    GameObject* getObject(Ogre::String oName);
    void addPlayer(Player* p);
    Player* getPlayer(Ogre::String pName);

    Sound *soundSystem;
    bool soundOn;
    
    void pause();
    bool paused();

    Overlay* overlay;
};

#endif
