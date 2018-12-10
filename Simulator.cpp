#include "Simulator.h"

Simulator::Simulator() {
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	overlappingPairCache = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver();
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,
			overlappingPairCache,
			solver,
			collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0.0, -98, 0.0));//0.0, 0.0));
	//Add collision shapes to reuse among rigid bodies

	soundSystem = new Sound();
	soundOn = false;

	overlay = new Overlay(this);
	isPaused = true;
}

void Simulator::addObject (GameObject* o) {
	objMap[o->getName()] = o;
    //TODO this crashes when we dont set object bodies
    objList.push_back(o);
	dynamicsWorld->addRigidBody(o->getBody());
}

bool Simulator::removeObject(GameObject*  o) {
	for(std::deque<GameObject*>::iterator i = objList.begin(); i != objList.end(); i++) {
		if((*i)->getName().compare(o->getName()) == 0) {
			objList.erase(i);
			objMap[o->getName().c_str()] = NULL;
			return true;
		}
	}
	return false;
}

void Simulator::printList() {
	printf("Printing object list \n\n");
	for(std::deque<GameObject*>::iterator i = objList.begin(); i != objList.end(); i++) {
		printf("%s\n", (*i)->getName().c_str());
	}
	printf("\nEnd of object list \n\n");
}

void Simulator::stepSimulation(const Ogre::Real elapsedTime, int maxSubSteps, const Ogre::Real fixedTimestep) {
    dynamicsWorld->stepSimulation(elapsedTime, maxSubSteps, fixedTimestep);
	for (unsigned int i = 0; i < objList.size(); i++) {
        // printf("Wow\n");
        // printf("Updating %s\n", objList[i]->getName().c_str());
		objList[i]->update(elapsedTime);
	}
}

btDiscreteDynamicsWorld* Simulator::getDynamicsWorld() {
    return dynamicsWorld;
}

GameObject* Simulator::getObject(Ogre::String oName) {
    return objMap[oName];
}

int Simulator::getObjectNumber() {
    return objList.size();
}

int Simulator::getObjectNumber(Ogre::String oName) {
    int totalCount = 0;
    for (unsigned int i = 0; i < objList.size(); i++) {
        if (objList[i]->getName().substr(0,oName.length()).compare(oName) == 0) {
            totalCount ++;
        }
    }
    return totalCount;
}

void Simulator::addPlayer(Player* p) {
	players[p->getName()] = p;
}

Player* Simulator::getPlayer(Ogre::String pName) {
    return players[pName];
}

/* Pauses or un-pauses the game */
void Simulator::pause() {
	isPaused = !isPaused;
	//soundSystem->playSound("bg_music");
}

/* Checks if the game is paused or not */
bool Simulator::paused() {
	return isPaused;
}

void Simulator::destroyWorld() {
	for(std::deque<GameObject*>::iterator i = objList.begin(); i != objList.end(); i++) {
		if(!(*i)->getName().compare("PlayerShooter") == 0) {
			(*i)->~GameObject();
			objList.erase(i);
            i--;
		}
	}
}

void Simulator::printMap() {
	printf("printing map \n");
	for(std::map<Ogre::String, GameObject*>::iterator i = objMap.begin(); i != objMap.end(); i++) {
		printf("%s\n", i->first.c_str());
	}
}

void Simulator::bulletTimer() {
	for(std::deque<GameObject*>::iterator i = objList.begin(); i != objList.end(); i++) {
        GameObject* obj = *i;
        Ogre::String name = obj->getName();
		if(name.substr(0, 11).compare("PlayerLaser") == 0 ||
            name.substr(0, 10).compare("EnemyLaser") == 0) {
			if(obj->increaseTimer()) {
				obj->~GameObject();
				objList.erase(i);
                objMap[name.c_str()] = NULL;
				i--;
			}
		}
	}
}

bool Simulator::hasKC() {
	Player* p1 = getPlayer("Player1");
	int level = p1->getLevel();
	int kc = p1->getKC();
	switch(level) {
		case 1:
			return true;
		case 2:
			return kc >= 1;
		case 3:
			return kc >= 2;
		case 4:
			return true;
		default:
			return true;
	}
}
