#include "Gun.h"

Gun::Gun(Ogre::String newName, Ogre::SceneManager* scnMgr,
    Ogre::Vector3 position, Ogre::Vector3 scale, Ogre::String material) {
    // Set variables.
    this->name = newName;
    this->sceneMgr = scnMgr;
    this->position = position;
    this->scale = scale;
    this->material = material;
    lastTime = 0.0f;

    // Set the entity.
    geom = sceneMgr->createEntity(name, "cube.mesh");
    geom->setCastShadows(true);
    if (material != "") {
        geom->setMaterialName(material);
    }

    // Set the rootNode.
    rootNode = sceneMgr->getRootSceneNode()->
        createChildSceneNode(name, Ogre::Vector3(position.x, position.y, position.z));
    rootNode->attachObject(geom);
    rootNode->scale(scale.x * 0.01f, scale.y * 0.01f, scale.z * 0.01f);
    rootNode->setPosition(position.x, position.y, position.z);
}

Gun::~Gun() {

}

void Gun::move(Ogre::Real x, Ogre::Real y, Ogre::Real z) {
    Ogre::Vector3 pPosition = rootNode->getPosition();
    float xNew = pPosition.x + x;
    float yNew = pPosition.y + y;
    float zNew = pPosition.z + z;
    this->setPosition(xNew, yNew, zNew);
}

void Gun::setPosition(float newX, float newY, float newZ) {
    rootNode->setPosition(newX, newY, newZ);
}

void Gun::setPosition(btVector3 newPosition) {
    rootNode->setPosition(newPosition.getX(), newPosition.getY(), newPosition.getZ());
}

void Gun::rotate(float wDir, float xDir, float yDir, float zDir) {
    rootNode->setOrientation(wDir, xDir, yDir, zDir);
}

void Gun::rotate(Ogre::Quaternion quat) {
    rootNode->setOrientation(quat);
}

// Specific game object update routine.
void Gun::update(float elapsedTime) {
    // lastTime += elapsedTime;
    // simulator->getDynamicsWorld()->contactTest(body, *cCallBack);
    // if (context->hit && (context->velNorm > 2.0 || context->velNorm < -2.0)
    //     && (lastTime > 0.5 || (context->lastBody != context->body && lastTime > 0.1))) {
    //     //Handle the hit
    //     lastTime = 0.0f;
    // }
    // context->hit = false;
}
