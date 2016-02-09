#pragma once

# include "ImageLoader/ImageLoader.h"
# include "libJPG/libjpeg/jpeglib.h"

class		JpgImageLoader : public ImageLoader
{
public:
	JpgImageLoader();
	~JpgImageLoader();
	bool	loadFromFile(const std::string &filename);
};

