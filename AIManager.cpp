#include "AIManager.h"

AIManager::AIManager(Ogre::SceneManager* sceneMgr, Simulator* simulator, Ogre::String material) {
    this->scnMgr = sceneMgr;
    this->simulator = simulator;
    this->material = material;
    this->numNodes = 0;
    this->player_position = Ogre::Vector3(-40.0f, -140.0f, 0.0f);
}

AIManager::~AIManager() {
    /* Delete all nodes in list */
    for(int i = 0; i < _nodes.size(); i++) {
        _nodes[i]->~Node();
    }
}

void AIManager::addNode(Ogre::Vector3 position) {
    Ogre::stringstream ss;
    ss << "node" << numNodes;
    _nodes.push_back(new Node(ss.str(), scnMgr, simulator,
    	position, 2.0f, "greenball"));
    numNodes++;
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

            if(_nodes[i] != _nodes[j] && abs(i_pos.x - j_pos.x) <= 20 && abs(i_pos.z - j_pos.z) <= 20 && abs(i_pos.z - j_pos.z) <= 20) {
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

    while (!frontier.empty()) {
        Node* current = frontier.top().second;
        frontier.pop();

        if (current == goal) {
            break;
        }

        neighbors = current->getNeighbors();
        for (Node* next : neighbors) {
            double new_cost = cost_so_far[current] + 1;
            if (cost_so_far.find(next) == cost_so_far.end()
                || new_cost < cost_so_far[next]) {
                cost_so_far[next] = new_cost;
                double priority = new_cost 
                    + manhattan_heuristic(next->getPosition(), goal->getPosition()); // might need to edit
                frontier.push(std::make_pair(priority, next));
                came_from[next] = current;
            }
        }
  }
}

std::vector<Node*> AIManager::reconstruct_path(std::unordered_map<Node*, Node*> came_from, Node* start, Node* goal) {
    /* Finds the optimal path to a node from another given node if given a map of nodes */
    Node* current = goal;
    std::vector<Node*> path;

    while(current != start) {
        path.push_back(current);
        current = came_from[current];
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
