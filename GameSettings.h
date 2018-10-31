#ifndef __GameSettings_h_
#define __GameSettings_h_

const float xMin = -34.0f, xMax = 34.0f, yMin = -35.0f, yMax = 35.0f, zMin = 0.0f, zMax = -300.0f;
const float xWWall = -50.0f, xEWall = 50.0f, yFWall = -50.0f, yCWall = 50.0f, zSWall = 10.0f, zNWall = -310.0f;

const float wallWidth = 100.0f, wallThickness = 20.0f, wallLength = 300.0f;
//float mass, float restitution, float friction, bool kinematic
//0.0f, 1.0f, 0.0f, false
const float wallMass = 0.0f, wallRestitution = 1.0f, wallFriction = 0.0f;
const bool wallKinematic = false;

const float ballMass = 1.0f, ballRestitution = .95f, ballFriction = 0.0f;
const bool ballKinematic = false;

const float paddleMass = 1.0f, paddleRestitution = 0.0f, paddleFriction = 0.0f;
const bool paddleKinematic = false;

const float shooterMass = 1.0f, shooterRestitution = 0.0f, shooterFriction = 0.0f;
const bool shooterKinematic = false;
const float enemyShooterMass = 0.0f;
const bool enemyShooterKinematic = true;

#endif
