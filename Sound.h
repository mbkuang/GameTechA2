#ifndef __Sound_h_
#define __Sound_h_

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_audio.h>
#include <iostream>
#include <Ogre.h>

class Sound {
public:
	Sound();
	~Sound();
	void playSound(Ogre::String);
protected:
	Mix_Music *bg_music;
    Mix_Chunk *wallSound;
    Mix_Chunk *paddleSound;
};

#endif
