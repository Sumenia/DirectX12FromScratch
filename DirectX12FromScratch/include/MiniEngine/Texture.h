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

	private:
		Magick::Blob		_blob;
	};
}