#include "AIManager.h"

AIManager::AIManager(Ogre::SceneManager* sceneMgr, Simulator* simulator, Ogre::String material) {
    this->scnMgr = sceneMgr;
    this->simulator = simulator;
    this->material = material;
    this->numNodes = 0;
    this->player_position = Ogre::Vector3(-40.0f, -140.0f, 0.0f);
    this->connectionDistance = 60;
    this->limit = 2000;
}

AIManager::~AIManager() {
    destroyNodes();
}

void AIManager::addNode(Ogre::Vector3 position) {
    Ogre::stringstream ss;
    ss << "node" << numNodes;
    _nodes.push_back(new Node(ss.str(), scnMgr, simulator,
    	position, 2.0f, "greenball"));
    numNodes++;
}

void AIManager::addNodesGridXZ(Ogre::Vector3 position, int x, int z) {    
    for(int i = position.x; i < (position.x + x + 1); i += connectionDistance) {
        for(int j = position.z; j < (position.z + z + 1); j += connectionDistance) {
            addNode(Ogre::Vector3(i, position.y, j));
        }
    }
}

void AIManager::addNodesGridXY(Ogre::Vector3 position, int x, int y) {    
    for(int i = position.x; i < (position.x + x + 1); i += connectionDistance) {
        for(int j = position.y; j < (position.y + y + 1); j += connectionDistance) {
            addNode(Ogre::Vector3(i, j, position.z));
        }
    }
}

void AIManager::addNodesGridYZ(Ogre::Vector3 position, int y, int z) {    
    for(int i = position.z; i < (position.z + z + 1); i += connectionDistance) {
        for(int j = position.y; j < (position.y + y + 1); j += connectionDistance) {
            addNode(Ogre::Vector3(position.x, j, i));
        }
    }
}

void AIManager::destroyNodes() {
    while (_nodes.size() > 0) {
        Node* n = _nodes[_nodes.size()-1];
        if (n != NULL) {
            n->~Node();
            _nodes.pop_back();
        }
        numNodes --;
    }
}

void AIManager::connectNodes(Node* node1, Node* node2) {
    node1->setConnection(node2);
}

void AIManager::connectAllNodes() {
    /* Connects all nodes to other nodes within a given distance, currently set to 20 */
    for(int i = 0; i < _nodes.size(); i++) {
            Ogre::Vector3 i_pos = _nodes[i]->getPosition();
        for(int j = 0; j < _nodes.size(); j++) {
            Ogre::Vector3 j_pos = _nodes[j]->getPosition();
            Ogre::Vector3 dist = i_pos - j_pos;
            if(dist.length() < connectionDistance + 1 && !collideWithWall(_nodes[i]->getPosition(), _nodes[j]->getPosition())) {
                connectNodes(_nodes[i], _nodes[j]);
            }
        }
    }
}

void AIManager::printAllNodeConnections() {
    for(int i = 0; i < _nodes.size(); i++) {
        _nodes[i]->printConnections();
    }
}

Node* AIManager::findNextNode(Node* start, Node* goal) {
    /* Return the next node an enemy should go to */

    std::unordered_map<Node*, Node*> came_from;
    a_star_search(came_from, start, goal);

    std::vector<Node*> path = reconstruct_path(came_from, start, goal);

    /* Print path , sanity check */
    // printf("Moving a frog \n");
    // for(int i = 0; i < path.size(); i++) {
    //     printf("Num: %d, Name of Node: %s, Position: %f, %f , %f\n",
    //     i, path[i]->getName().c_str(), path[i]->getPosition().x, path[i]->getPosition().y, path[i]->getPosition().z);
    // }

    return path[1] != NULL ? path[1] : path[0];
}

void AIManager::a_star_search(std::unordered_map<Node*, Node*>& came_from, Node* start, Node* goal) {
    /* Use the A* algorithm to find organize the given map */
    std::unordered_map<Node*, double> cost_so_far;

    std::priority_queue<std::pair<double, Node*>> frontier;
    std::vector<Node*> neighbors;
    frontier.push(std::make_pair(0, start));

    came_from[start] = start;
    cost_so_far[start] = 0;

    int count = 0;

    while (!frontier.empty()) {
        Node* current = frontier.top().second;
        frontier.pop();

        if (current == goal || count == limit) {  
            break;
        }

        neighbors = current->getNeighbors();
        for (Node* next : neighbors) {
            double new_cost = cost_so_far[current] + 1;
            if (cost_so_far.find(next) == cost_so_far.end()
                || new_cost < cost_so_far[next]) {
                cost_so_far[next] = new_cost;
                double priority = new_cost
                    + 10 * manhattan_heuristic(next->getPosition(), goal->getPosition());
                frontier.push(std::make_pair(priority, next));
                came_from[next] = current;
            }
        }
        count++;
  }
}

std::vector<Node*> AIManager::reconstruct_path(std::unordered_map<Node*, Node*> came_from, Node* start, Node* goal) {
    /* Finds the optimal path to a node from another given node if given a map of nodes */
    Node* current = goal;
    std::vector<Node*> path;

    int count = 0;

    while(current != start && count != limit) {
        path.push_back(current);
        current = came_from[current];
        count++;
    } 
    
    if(count == limit) {
        std::vector<Node*> subPath;
        std::vector<Node*> neighbors = start->getNeighbors();
        Node* closest = findNodeClosestList(goal->getPosition(), neighbors);
        subPath.push_back(closest);
        return subPath;
    }

    path.push_back(start);
    std::reverse(path.begin(),path.end());
    return path;
}


Node* AIManager::findNodeClosest(Ogre::Vector3 position) {
    /* Find node closest to a given position */
    Node* ret = NULL;
    float min_dist = std::numeric_limits<float>::max();
    float dist;

    for(int i = 0; i < _nodes.size(); i++) {
        if((dist = distance(position, _nodes[i]->getPosition())) < min_dist) {
            min_dist = dist;
            ret = _nodes[i];
        }
    }

    return ret;
}

Node* AIManager::findNodeClosestList(Ogre::Vector3 position, std::vector<Node*> nodes) {
    /* Find node closest to a given position */
    Node* ret = NULL;
    float min_dist = std::numeric_limits<float>::max();
    float dist;

    for(int i = 0; i < nodes.size(); i++) {
        if((dist = distance(position, nodes[i]->getPosition())) < min_dist) {
            min_dist = dist;
            ret = nodes[i];
        }
    }

    return ret;
}

Node* AIManager::findNodeClosestPlayer(Ogre::Vector3 position, Node* nodeAt) {
    /* Find node closest to a given position */
    Node* ret = NULL;
    float min_dist = std::numeric_limits<float>::max();
    float dist;

    for(int i = 0; i < _nodes.size(); i++) {
        if((dist = distance(position, _nodes[i]->getPosition())) < min_dist && ( !(_nodes[i]->isOccupied()) || (nodeAt == _nodes[i] ) ) ) {
            min_dist = dist;
            ret = _nodes[i];
        }
    }

    return ret;
}

float AIManager::distance(Ogre::Vector3 a, Ogre::Vector3 b) {
    /* Find relative distance from position a to position b */
        return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z);
}

float AIManager::manhattan_heuristic(Ogre::Vector3 a, Ogre::Vector3 b) {
    /* Find manhattan distance in for 2 vectors */
    return abs(a.x - b.x) + abs(a.y - b.y) + abs(a.z - b.z);
}

void AIManager::setPlayerPosition(Ogre::Vector3 pos) {
    player_position = pos;
}

Ogre::Vector3 AIManager::getPlayerPosition() {
    return player_position;
}

/* Wall collision */


void AIManager::addWall(float a, float b, float c, float d) {
    Point p1;
    Point p2; 
    p1.x = a;
    p1.y = b;
    p2.x = c;
    p2.y = d;
    _walls.push_back(std::make_pair(p1, p2));
}


bool AIManager::collideWithWall(Ogre::Vector3 pos1, Ogre::Vector3 pos2) {
    /* TODO Check if the the connection of two nodes collides with a wall*/ 
    Point p1;
    Point p2;

    p1.x = pos1.x;
    p1.y = pos1.z;

    p2.x = pos2.x;
    p2.y = pos2.z;

    for(int i = 0; i < _walls.size(); i++) {
        if(doIntersect(p1, p2, _walls[i].first, _walls[i].second)) {
            return true;
        }
    }

    return false;
}

bool AIManager::onSegment(Point p, Point q, Point r) 
{ 
    if (q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) && 
        q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y)) 
       return true; 
  
    return false; 
} 

float AIManager::orientation(Point p, Point q, Point r) 
{ 
    float val = (q.y - p.y) * (r.x - q.x) - 
              (q.x - p.x) * (r.y - q.y); 
  
    if (val == 0) return 0;
  
    return (val > 0)? 1: 2;
} 

bool AIManager::doIntersect(Point p1, Point q1, Point p2, Point q2) 
{ 
    // Find the four orientations needed for general and 
    // special cases 
    float o1 = orientation(p1, q1, p2); 
    float o2 = orientation(p1, q1, q2); 
    float o3 = orientation(p2, q2, p1); 
    float o4 = orientation(p2, q2, q1); 
  
    // General case 
    if (o1 != o2 && o3 != o4) 
        return true; 
  
    // Special Cases 
    // p1, q1 and p2 are colinear and p2 lies on segment p1q1 
    if (o1 == 0 && onSegment(p1, p2, q1)) return true; 
  
    // p1, q1 and q2 are colinear and q2 lies on segment p1q1 
    if (o2 == 0 && onSegment(p1, q2, q1)) return true; 
  
    // p2, q2 and p1 are colinear and p1 lies on segment p2q2 
    if (o3 == 0 && onSegment(p2, p1, q2)) return true; 
  
     // p2, q2 and q1 are colinear and q1 lies on segment p2q2 
    if (o4 == 0 && onSegment(p2, q1, q2)) return true; 
  
    return false; // Doesn't fall in any of the above cases 
} 
