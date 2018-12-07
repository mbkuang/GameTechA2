#ifndef __GameSettings_h_
#define __GameSettings_h_

const float xMin = -34.0f, xMax = 34.0f, yMin = -35.0f, yMax = 35.0f, zMin = 0.0f, zMax = -300.0f;

const float xWWall = -150.0f, xEWall = 150.0f, yFWall = -150.0f, yCWall = 150.0f, zSWall = -150.0f, zNWall = 150.0f;
const float wallWidth = 300.0f, wallThickness = 20.0f, wallLength = 300.0f;
const float wallMass = 0.0f, wallRestitution = 1.0f, wallFriction = 0.0f;
const bool wallKinematic = false;

const float ballMass = 1.0f, ballRestitution = .95f, ballFriction = 0.0f;
const bool ballKinematic = false;

const float shooterMass = 1.0f, shooterRestitution = 0.0f, shooterFriction = 0.0f;
const bool shooterKinematic = false;
const float enemyShooterMass = 0.0f;
const bool enemyShooterKinematic = true;

const float doorMass = 0.0f, doorRestitution = 0.0f, doorFriction = 0.0f;
const bool doorKinematic = false;

const float jumpPeriod = 1;

const float laserSpeed = 462.0f;

#endif
