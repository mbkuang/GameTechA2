#include <AIManager.h>

AIManager::AIManager(Simulator* sim) {
    movementSpeed = 30.0f;
    simulator = sim;
    level = 0;
}

AIManager::AIManager(Ogre::SceneManager* scnMgr, Simulator* sim, Paddle* pad, Ball* nBall) {
    movementSpeed = 30.0f;
    sceneMgr = scnMgr;
    simulator = sim;
    paddle = pad;
    ball = nBall;
    level = 0;
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
    float worry = (bPosition.getZ()*2 - zPos - level)/zPos;
    worry = std::max(0.0f, worry);

    paddle->move(
        xDir * worry * movementSpeed * fe.timeSinceLastFrame,
        yDir * worry * movementSpeed * fe.timeSinceLastFrame,
        0.0f
    );
}

void AIManager::shoot() {
    int random = rand() % 10;
    if(random > 2)
        return;

    Player* cpu = simulator->getPlayer("CPU");
    if(!cpu->hasFired()) {
        GameObject* paddle = simulator->getObject("CPUPaddle");
        Ogre::Vector3 location = (Ogre::Vector3) paddle->getPosition();

        if(cpu->getNumShots() == 0) {
            Ball* laser = new Ball("cpulaser", sceneMgr, simulator,
            Ogre::Vector3(location.x, location.y, location.z+20), 0.5f,
            "greenball", ballMass, ballRestitution, ballFriction, ballKinematic);
            laser->setVelocity(btVector3(0, 0, 200));
        }
        else {
            Ball* laser = (Ball*) simulator->getObject("cpulaser");
            laser->setPosition(btVector3(location.x, location.y, location.z+20));
            laser->setVelocity(btVector3(0, 0, 200));
        }
        cpu->shot();
        simulator->soundSystem->playSound("laserSound");
    }
}
