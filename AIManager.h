#ifndef __AIManager_h_
#define __AIManager_h_

#include "Node.h"
#include <Ogre.h>
#include "Simulator.h"
#include <vector>
#include <unordered_map>
#include <queue>
#include <utility>      // std::pair, std::make_pair
#include <cmath>
#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>    // std::reverse
#include <unordered_set>

class AIManager {
    struct Point 
    { 
        float x; 
        float y; 
    }; 

    protected:
        std::vector<Node*> _nodes;
        Ogre::SceneManager* scnMgr;
        Simulator* simulator;
        Ogre::String material;
        int numNodes;
        Ogre::Vector3 player_position;
        float connectionDistance;
        double limit;
        std::vector<std::pair<Point, Point>> _walls;

    public:
        AIManager(Ogre::SceneManager* sceneMgr, Simulator* simulator, Ogre::String material);

        ~AIManager();

        void addNode(Ogre::Vector3 position);

        void destroyNodes();

        void connectNodes(Node* node1, Node* node2);

        Node* findNextNode(Node* start, Node* goal);

        void connectAllNodes();

        void printAllNodeConnections();

        Node* findNodeClosest(Ogre::Vector3 position);

        Node* findNodeClosestPlayer(Ogre::Vector3 position, Node* nodeAt);

        Node* findNodeClosestList(Ogre::Vector3 position, std::vector<Node*> nodes);

        float distance(Ogre::Vector3 a, Ogre::Vector3 b);

        float manhattan_heuristic(Ogre::Vector3 a, Ogre::Vector3 b);

        void a_star_search(std::unordered_map<Node*, Node*>& came_from, Node* start, Node* goal);

        std::vector<Node*> reconstruct_path(std::unordered_map<Node*, Node*> came_from, Node* start, Node* goal);

        void setPlayerPosition(Ogre::Vector3 pos);

        Ogre::Vector3 getPlayerPosition();

        void addNodesGridXZ(Ogre::Vector3 position, int x, int z);

        void addNodesGridXY(Ogre::Vector3 position, int x, int y);

        void addNodesGridYZ(Ogre::Vector3 position, int y, int z);

        void addWall(float a, float b, float c, float d);

        bool collideWithWall(Ogre::Vector3 pos1, Ogre::Vector3 pos2);

        bool onSegment(Point p, Point q, Point r);

        float orientation(Point p, Point q, Point r);

        bool doIntersect(Point p1, Point q1, Point p2, Point q2);


};

#endif
