#include "Gun.h"

Gun::Gun(Ogre::String newName, Ogre::SceneManager* scnMgr,
    Ogre::Vector3 position, Ogre::Vector3 scale, Ogre::String material) {
    // Set variables.
    this->name = newName;
    this->sceneMgr = scnMgr;
    this->position = position;
    this->scale = scale;
    this->material = material;

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

Ogre::SceneNode* Gun::getRootNode() {
    return this->rootNode;
}

void Gun::setPosition(btVector3 newPosition) {
    rootNode->setPosition(newPosition.getX(), newPosition.getY(), newPosition.getZ());
}

btVector3 Gun::getPosition() {
    Ogre::Vector3 pos = this->rootNode->getPosition();
    return btVector3(pos.x, pos.y, pos.z);
}

void Gun::rotate(Ogre::Quaternion quat) {
    rootNode->setOrientation(quat);
}
