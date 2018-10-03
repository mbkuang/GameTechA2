class Ball : public GameObject {
public:
    Ball(Ogre::SceneManager* scnMgr);
    ~Ball();
    void move(const Ogre::FrameEvent& evt);
    Ogre::SceneNode* getNode() {return rootNode;}
}
