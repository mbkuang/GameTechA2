#include <Ogre.h>
#include <OgreEntity.h>
#include <OgreVector3.h>
#include <OgreSceneManager.h>
#include <Ball.h>

Ball::Ball(Ogre::SceneManager* scnMgr) {
    Ogre::Entity* ball = scnMgr->createEntity("Sphere", "sphere.mesh");
    ball->setCastShadows(true);
    rootNode = scnMgr->getRootSceneNode()
        ->createChildSceneNode("Ball", Ogre::Vector3(0,0,0));
    rootNode->attachObject(ball);
    rootNode->scale(0.1f,0.1f,0.1f);
    rootNode->setPosition(0,0,0);
    bRadius = 15.0f;
    bDirection = Ogre::Vector3(
        (rand()%100-50)/100.0f,
        (rand()%100-50)/100.0f,
        (rand()%100-50)/100.0f);
    bDirection.normalise();
    bSpeed = (rand()%200+50)/1.0f;

    //TODO Set the rigid Body
    body = NULL;
}

Ogre::Vector3 Ball::getPosition() {
    return rootNode->getPosition();
}

Ogre::Real Ball::getRadius() {
    return bRadius;
}
