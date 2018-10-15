#ifndef __GameSettings_h_
#define __GameSettings_h_

const float xMin = -34, xMax = 34, yMin = -35, yMax = 35, zMin = 0, zMax = -300;
const float xWWall = -50, xEWall = 50, yFWall = -50, yCWall = 50, zSWall = 10, zNWall = -310;

//float mass, float restitution, float friction, bool kinematic
//0.0f, 1.0f, 0.0f, false
const float wallMass = 0.0f, wallRestitution = 1.0f, wallFriction = 0.0f;
const bool wallKinematic = false;

const float ballMass = 1.0f, ballRestitution = 1.0f, ballFriction = 0.0f;
const bool ballKinematic = false;

const float paddleMass = 0.0f, paddleRestitution = 1.0f, paddleFriction = 0.0f;
const bool paddleKinematic = false;

#endif
