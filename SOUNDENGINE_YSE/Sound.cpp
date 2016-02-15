#include "Sound.hh"

Sound::Sound()
{
	_sound = new YSE::sound();
}

Sound::Sound(const std::string path)
{
	Sound();
	_sound->create(path.c_str());
}

Sound::~Sound()
{
}

bool Sound::loadSound(const std::string path)
{
	_sound->create(path.c_str());
	return (true);
}

void Sound::play()
{
	_sound->play();
}