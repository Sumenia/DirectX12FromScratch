#pragma once

# include "ImageLoader.h"

class				BmpImageLoader : public ImageLoader
{
public:
    BmpImageLoader();
    ~BmpImageLoader();
	static bool		isBmp(FILE *file);
    bool			loadFromFile(FILE *file);
};

