#pragma once

#include "ImageLoader/PngImageLoader.h"

namespace MiniEngine
{
	class Texture
	{
	public:
		Texture();
		virtual ~Texture();

		virtual bool			loadFromFile(const std::string &filename);
		ImageLoader				*getImage();

	protected:
		ImageLoader				*_loader;
	};
}