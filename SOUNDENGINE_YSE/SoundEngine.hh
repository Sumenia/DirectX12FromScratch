#ifndef SOUNDENGINE_HH_
# define SOUNDENGINE_HH_

#include "yse.hpp"

class SoundEngine
{
public:
	SoundEngine();
	~SoundEngine();

	static void init();
	static void update();
	static void close();
};

#endif // !SoundEngine_HH_
