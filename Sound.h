#ifndef __Sound_h_
#define __Sound_h_

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_audio.h>
#include <iostream>
#include <Ogre.h>
#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>

const int MAX_VOLUME = 128;

class Sound {
public:
	Sound();
	~Sound();
	void playSound(Ogre::String);
	void volumeUp();
	void volumeDown();
	int currentVolume;
	void shutOffSound();
protected:
	Mix_Music *bg_music;
    Mix_Chunk *wallSound;
    Mix_Chunk *paddleSound;
    Mix_Chunk *scoreSound;
    Mix_Chunk *laserSound;
    Mix_Chunk *deathSound;
    bool soundOn;
};

#endif
