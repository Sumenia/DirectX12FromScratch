#include <iostream>
#include "MiniEngine/Texture.h"

using namespace MiniEngine;

Texture::Texture() {}

Texture::~Texture() {}

bool				Texture::loadFromFile(const std::string &filename)
{
	_loader = new PngImageLoader;

	return (_loader->loadFromFile(filename));
}

ImageLoader			*Texture::getImage()
{
	return _loader;
}