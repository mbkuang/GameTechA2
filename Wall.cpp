#include <Ogre.h>
#include <OgreEntity.h>
#include <OgreVector3.h>
#include <OgreSceneManager.h>
#include <Wall.h>

Wall::Wall(Ogre::SceneManager* scnMgr) {
    Ogre::Entity* wall = scnMgr->createEntity("cube.mesh");
    rootNode = scnMgr->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(0,0,0));
    rootNode->setScale(1,1,1);
    rootNode->attachObject(wall);

    //TODO Set the rigid Body
    body = NULL;
}
