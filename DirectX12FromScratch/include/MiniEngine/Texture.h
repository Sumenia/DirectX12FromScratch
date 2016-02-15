#pragma once

#include "ImageLoader/PngImageLoader.h"
#include "ImageLoader/JpgImageLoader.h"

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