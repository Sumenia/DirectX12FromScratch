#pragma once

#include "ImageLoader/PngImageLoader.h"

namespace MiniEngine
{
	class Texture
	{
	public:
		Texture();
		~Texture();
		bool					loadFromFile(const std::string &filename);
		AImageLoader			*getImage();

	private:
		AImageLoader			*_loader;
	};
}