#pragma once

# include <Magick++.h>

namespace MiniEngine
{
	class Texture
	{
	public:
		Texture();
		~Texture();
		bool loadFromFile(const std::string &filename);
		const Magick::Blob	&getblob();

	protected:
		Magick::Blob		_blob;
	};
}