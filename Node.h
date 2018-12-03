#ifndef __Node_h_
#define __Node_h_

#include "GameObject.h"
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

class Node : public GameObject {
    protected:
        Ogre::Vector3 position;
        float radius;
        Ogre::String material;
        std::vector<Node*> _connections;
        bool occupied;

    public:
        inline bool operator == (const Node& n) const {
		    return position == n.position;
        }

        inline bool operator != (const Node& n) const {
		    return *this == n;
        }

        Node(Ogre::String name, Ogre::SceneManager* sceneMgr, Simulator* simulator,

    	Ogre::Vector3 position, float radius, Ogre::String material);

        void setConnection(Node* n);

        void printConnections();

        Ogre::Vector3 getPosition();
        
        std::vector<Node*> getNeighbors();

        void occupy(bool inUse);

        bool isOccupied();
};

#endif
