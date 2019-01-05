#include "Sound.h"

Sound::Sound()
{

}

Sound::Sound(int channel, Mix_Chunk *chunk, int loops)
{
	SetSound(channel, chunk, loops);
}
void Sound::SetSound(int channel, Mix_Chunk *chunk, int loops)
{
	this->channel = channel;
	this->chunk = chunk;
	this->loops = loops;
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1)
	{
		printf("%s", Mix_GetError());
	}
}
void Sound::playSound()
{
	if (!Mix_Playing(channel))
		Mix_PlayChannel(-1, chunk, loops);
}
void Sound::PauseSound()
{
	Mix_Pause(-1);
}

void Sound::ResumeSound()
{
	Mix_Resume(-1);
}
void Sound::StopSound()
{
	Mix_HaltChannel(-1);
}

Sound::~Sound()
{
}
