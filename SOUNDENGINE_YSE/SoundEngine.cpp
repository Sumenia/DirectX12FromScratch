#include "SoundEngine.hh"

SoundEngine::SoundEngine()
{}

SoundEngine::~SoundEngine()
{}

void SoundEngine::init()
{
	YSE::System().init();
}

void SoundEngine::update()
{
	YSE::System().update();
}

void SoundEngine::close()
{
	YSE::System().close();
}