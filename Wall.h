class Wall : public GameObject {
public:
    Wall(Ogre::SceneManager* scnMgr);
    ~Wall();
    Ogre::SceneNode* getNode() {return rootNode;}
}
