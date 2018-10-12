#include "Sound.h"

Sound::Sound() {
	SDL_Init(SDL_INIT_AUDIO);
	Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);

	bg_music = Mix_LoadMUS("media/mountains.mp3");
	Mix_PlayMusic(bg_music, -1);

	currentVolume = MAX_VOLUME/2;

	wallSound = Mix_LoadWAV("media/click.wav");
	paddleSound = Mix_LoadWAV("media/Pew.wav");
	scoreSound = Mix_LoadWAV("media/smw_1-up.wav");
}

void Sound::playSound(Ogre::String soundType) {
	if(soundType.compare("bg_music") == 0) {
		if(Mix_PlayingMusic() == 0)
			Mix_PlayMusic(bg_music, -1);
		else if (Mix_PausedMusic() == 1)
        	Mix_ResumeMusic();
    	else
        	Mix_PauseMusic();
	}
	else if(soundType.compare("paddleSound") == 0) {
		Mix_PlayChannel(-1, paddleSound, 0);
	}
	else if(soundType.compare("wallSound") == 0) {
		Mix_PlayChannel(-1, wallSound, 0);
	}
	else
		Mix_PlayChannel(-1, scoreSound, 0);
}

void Sound::volumeDown() {
	currentVolume -= 2;
	Mix_VolumeMusic(currentVolume);
}

void Sound::volumeUp() {
	currentVolume += 2;
	Mix_VolumeMusic(currentVolume);
}