#include <iostream>
#include "MiniEngine/Texture.h"

using namespace MiniEngine;

Texture::Texture() {}

Texture::~Texture()
{
    // delete _loader; TO-DO: Uncomment this
    _loader = nullptr;
}

bool  Texture::loadFromFile(const std::string &filename)
{
	errno_t		err;
	FILE		*file;
	char		error[4096];
	bool		res = false;

	_loader = nullptr;
	err = fopen_s(&file, filename.c_str(), "rb");
	if (err)
	{
		strerror_s(error, 4096, err);
		std::cerr << "Error opening file " << filename << " : " << error << std::endl;
		return (false);
	}

	if (JpgImageLoader::isJpg(file))
		_loader = new JpgImageLoader;
	else if (PngImageLoader::isPng(file))
		_loader = new PngImageLoader;
	else if (BmpImageLoader::isBmp(file))
		_loader = new BmpImageLoader;

	if (_loader)
		res = _loader->loadFromFile(file);

	if (!res)
		std::cerr << "wrong image format" << std::endl;

	fclose(file);

	return (res);
}

ImageLoader	*Texture::getImage()
{
	return _loader;
}