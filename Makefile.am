noinst_HEADERS = GameObject.h OgreMotionState.h Player.h Simulator.h Wall.h BaseApplication.h TutorialApplication.h Sound.h ContactSensorCallback.h GameSettings.h Overlay.h Shooter.h Gun.h Laser.h Bird.h Node.h AIManager.h Frog.h Door.h Spawner.h

bin_PROGRAMS = assignment2
assignment2_CPPFLAGS = -I$(top_srcdir) -std=c++11
assignment2_SOURCES = GameObject.cpp Player.cpp Simulator.cpp Wall.cpp BaseApplication.cpp TutorialApplication.cpp OgreMotionState.cpp Sound.cpp Overlay.cpp Shooter.cpp Gun.cpp Laser.cpp Bird.cpp Node.cpp AIManager.cpp Frog.cpp Door.cpp Spawner.cpp
assignment2_CXXFLAGS = $(OGRE_CFLAGS) $(OIS_CFLAGS) $(bullet_CFLAGS) $(CEGUI_CFLAGS) $(CEGUI_OGRE_CFLAGS) $(SDL_mixer_CFLAGS) $(SDL_net_CFLAGS)
assignment2_LDADD = $(OGRE_LIBS) $(OIS_LIBS) $(bullet_LIBS) $(CEGUI_LIBS) $(CEGUI_OGRE_LIBS) $(SDL_mixer_LIBS) $(SDL_net_LIBS)
assignment2_LDFLAGS = -lOgreOverlay -lboost_system -lSDL -lSDL_mixer -lSDL_net

EXTRA_DIST = buildit makeit
AUTOMAKE_OPTIONS = foreign
