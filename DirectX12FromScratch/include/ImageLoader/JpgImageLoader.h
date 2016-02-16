#pragma once

# include "ImageLoader/ImageLoader.h"
# include "libJPG/libjpeg/jpeglib.h"
# include "libJPG/libjpeg/jerror.h"

class		JpgImageLoader : public ImageLoader
{
public:
	JpgImageLoader();
	~JpgImageLoader();
	bool			loadFromFile(FILE *file);
	static bool		isJpg(FILE *file);
};

