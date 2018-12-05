#include "Node.h"

Node::Node(Ogre::String newName, Ogre::SceneManager* scnMgr, Simulator* sim,
    Ogre::Vector3 position, float radius, Ogre::String material)
    : GameObject(newName, scnMgr, sim) {
    // Set variables.
    this->position = position;
    this->radius = radius;
    this->material = material;
    this->occupied = false;

    // Set the entity.
    geom = sceneMgr->createEntity(name, "sphere.mesh");
    geom->setCastShadows(true);
    if (material != "") {
        geom->setMaterialName(material);
    }

    // Set the rootNode.
    rootNode = sceneMgr->getRootSceneNode()
        ->createChildSceneNode(name, Ogre::Vector3(position.x, position.y, position.z));
    rootNode->attachObject(geom);
    rootNode->scale(radius * 0.01f, radius * 0.01f, radius * 0.01f);
    rootNode->setPosition(position.x, position.y, position.z);
}

Node::~Node() {
    
}

void Node::setConnection(Node* n) {
    _connections.push_back(n);
}

void Node::printConnections() {
    printf("Listing for %s : at Position: %f, %f, %f\n", this->getName().c_str(), position.x, position.y, position.z);
    for(int i = 0; i < _connections.size(); i++) {
        printf("%d : %s  at Position: %f, %f, %f\n", i, _connections[i]->getName().c_str(),
        _connections[i]->getPosition().x, _connections[i]->getPosition().y, _connections[i]->getPosition().z);
    }
    printf("\n");
}

Ogre::Vector3 Node::getPosition() {
    return position;
}

std::vector<Node*> Node::getNeighbors() {
    return _connections;
}

void Node::occupy(bool occupy) {
    this->occupied = occupy;
}

bool Node::isOccupied() {
    return this->occupied;
}
