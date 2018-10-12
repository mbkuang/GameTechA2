#include <AIManager.h>

AIManager::AIManager(Simulator* sim) {
    movementSpeed = 40.0f;
    simulator = sim;
}

AIManager::AIManager(Ogre::SceneManager* scnMgr, Simulator* sim, Paddle* pad, Ball* nBall) {
    movementSpeed = 40.0f;
    sceneMgr = scnMgr;
    simulator = sim;
    paddle = pad;
    ball = nBall;
}

AIManager::~AIManager() {

}

void AIManager::update(Ogre::SceneManager* scnMgr, Simulator* sim, Paddle* pad, Ball* nBall) {
    sceneMgr = scnMgr;
    simulator = sim;
    paddle = pad;
    ball = nBall;
}

void AIManager::move(const Ogre::FrameEvent& fe) {
    // Move the CPU paddle towards the ball
    btVector3 bPosition = ball->getPosition();
    btVector3 pPosition = paddle->getPosition();
    float xDir = bPosition.getX() - pPosition.getX();
    xDir = xDir < 0 ? -1 : 1;
    float yDir = bPosition.getY() - pPosition.getY();
    yDir = yDir < 0 ? -1 : 1;

    float zPos = pPosition.getZ();
    float worry = (bPosition.getZ()*2 - zPos)/zPos;
    worry = std::max(0.0f, worry);

    paddle->move(
        xDir * worry * movementSpeed * fe.timeSinceLastFrame,
        yDir * worry * movementSpeed * fe.timeSinceLastFrame,
        0.0f
    );
}
