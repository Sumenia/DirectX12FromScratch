#include <iostream>
#include "MiniEngine/Texture.h"

using namespace MiniEngine;

Texture::Texture() {}

Texture::~Texture() {}

bool				Texture::loadFromFile(const std::string &filename)
{
	try
	{
		Magick::Image	image;

		image.read(filename);
		image.magick("RGBA");
		image.write(&_blob);
	}
	catch (Magick::Exception &error_)
	{
		std::cout << error_.what() << std::endl;
		return (false);
	}

	return (true);
}

const Magick::Blob	&Texture::getblob()
{
	return _blob;
}