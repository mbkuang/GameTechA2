#include <Wall.h>

Wall::Wall(Ogre::String newName, Ogre::SceneManager* scnMgr, Simulator* sim,
    float xPosition, float yPosition, float zPosition,
    float xScale, float yScale, float zScale,
    Ogre::String material) :
    GameObject(newName, scnMgr, sim) {
    // Set the entity.
    geom = sceneMgr->createEntity(name, "cube.mesh");
    geom->setCastShadows(true);
    if (material != "") {
        geom->setMaterialName(material);
    }
        
    // Set the rootNode.
    rootNode = sceneMgr->getRootSceneNode()->
        createChildSceneNode(name, Ogre::Vector3(xPosition, yPosition, zPosition));
    rootNode->attachObject(geom);
    rootNode->scale(xScale * 0.01f, yScale * 0.01f, zScale * 0.01f);
    rootNode->setPosition(xPosition, yPosition, zPosition);

    // Set the rigid body.
    transform.setOrigin(btVector3(xPosition, yPosition, zPosition));
    shape = new btBoxShape(btVector3(xScale * 0.5f, yScale * 0.5f, zScale * 0.5f));
    motionState = new OgreMotionState(transform, rootNode);
    mass = 0.0f;
    restitution = 1.0f;
    friction = 0.0f;

    addToSimulator();
}

Wall::~Wall() {
    //TODO:
}

// Specific game object update routine.
void Wall::update(float elapsedTime) {
    lastTime += elapsedTime;
    simulator->getDynamicsWorld()->contactTest(body, *cCallBack);
    if (context->hit && (context->velNorm > 2.0 || context->velNorm < -2.0) 
        && (lastTime > 0.5 || (context->lastBody != context->body && lastTime > 0.1))) {
        //Handle the hit
        lastTime = 0.0f;
    }
    context->hit = false;
}
