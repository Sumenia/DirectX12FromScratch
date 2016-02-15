#pragma once

# include "ImageLoader.h"
# include <Windows.h>

class           BmpImageLoader : public ImageLoader
{
public:
    BmpImageLoader();
    ~BmpImageLoader();
    bool        loadFromFile(const std::string &filename);
};

