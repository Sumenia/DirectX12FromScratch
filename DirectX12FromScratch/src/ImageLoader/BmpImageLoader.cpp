#include <iostream>
#include "ImageLoader/BmpImageLoader.h"

BmpImageLoader::BmpImageLoader() : ImageLoader(EImageFormat::Type::BMP) {}

BmpImageLoader::~BmpImageLoader() {}

bool                    BmpImageLoader::loadFromFile(const std::string &filename)
{
    FILE                *file;
    errno_t             err;
    char                error[4096];
    BITMAPFILEHEADER    fileHeader;
    BITMAPINFOHEADER    infoHeader;
    

    err = fopen_s(&file, filename.c_str(), "rb");
    if (err)
    {
        strerror_s(error, 4096, err);
        std::cerr << "Error opening file " << filename << " : " << error << std::endl;
        return (false);
    }

    fread(&fileHeader, sizeof(BITMAPFILEHEADER), 1, file);

    if (fileHeader.bfType != 0x4D42)
    {
        std::cerr << "Error : " << filename << " is not a BMP file " << std::endl;
        fclose(file);
        return (false);
    }

    fread(&infoHeader, sizeof(BITMAPINFOHEADER), 1, file);

    fseek(file, fileHeader.bfOffBits, SEEK_SET);

    _rgbFormat = ERGBFormat::Type::RGB;
    _bitDepth = infoHeader.biBitCount;
    _height = infoHeader.biHeight;
    _width = infoHeader.biWidth;
    _length = infoHeader.biSizeImage;
    _data = new unsigned char[infoHeader.biSizeImage];

    fread(_data, infoHeader.biSizeImage, 1, file);

    if (!_data || !_length)
    {
        std::cerr << "Error : " << filename << " when reading file" << std::endl;
        fclose(file);
        return (false);
    }

    swapToRGB();

    fclose(file);

    return (true);
}

void                    BmpImageLoader::swapToRGB()
{
    unsigned char       last;
    int                 cursor;

    for (cursor = 0; cursor < _length; cursor += 3)
    {
        last = ((unsigned char *)_data)[cursor];
        ((unsigned char *)_data)[cursor] = ((unsigned char *)_data)[cursor + 2];
        ((unsigned char *)_data)[cursor + 2] = last;
    }
}