#pragma once

#include <SDL.h>
#include <SDL_mixer.h>
#include <iostream>
using namespace std;
class Sound
{
	int channel;
	Mix_Chunk * chunk;
	int loops;
public:
	Sound();
	Sound(int channel, Mix_Chunk *chunk, int loops);
	void SetSound(int channel, Mix_Chunk *chunk, int loops);
	void playSound();
	void PauseSound();
	void ResumeSound();
	void StopSound();
	~Sound();
};

