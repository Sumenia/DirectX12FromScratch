#pragma once

#include <string>
#include "yse.hpp"

class Sound
{
public:
	Sound(const std::string path);
	Sound();
	~Sound();

	void play();
	bool loadSound(const std::string path);

private:
	YSE::sound *_sound;
};