#ifndef __Simulator_h_
#define __Simulator_h_

#ifndef __Simulator_h_DEFINED
    #define __Simulator_h_DEFINED
    class Simulator;
    #include "GameObject.h"
#endif

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>
#include <Ogre.h>

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

public:
	Simulator();
	~Simulator();
	void addObject(GameObject* o);
	//bool removeObject(GameObject* o);
	void stepSimulation(const Ogre::Real elapsedTime, int maxSubSteps = 1, const Ogre::Real fixedTimestep = 1.0f/60.0f);
    //btDiscreteDynamicsWorld* getDynamicsWorld();
    //std::vector<btCollisionShape*> getCollisionShapes();
    //int getCollisionObjectCount();
    GameObject* getObject(Ogre::String oName);
};

#endif
