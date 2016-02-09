#pragma once

# include "ImageLoader/IImageLoader.h"
# include "libJPG/libjpeg/jpeglib.h"

class		JpgImageLoader : public IImageLoader
{
public:
	JpgImageLoader();
	~JpgImageLoader();
	bool	loadFromFile(const std::string &filename);
};

