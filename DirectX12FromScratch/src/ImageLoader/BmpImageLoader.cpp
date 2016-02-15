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
    unsigned char       *buffer;
    int                 cursor;
    int                 pixel;

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

    _rgbFormat = ERGBFormat::Type::RGBA;
    _bitDepth = infoHeader.biBitCount;
    _height = infoHeader.biHeight;
    _width = infoHeader.biWidth;
    _length = _width * _height * 4;
    _data = new unsigned char[_length];

    buffer = new unsigned char[infoHeader.biSizeImage];
    fread(buffer, infoHeader.biSizeImage, 1, file);

    if (!buffer || !infoHeader.biSizeImage)
    {
        std::cerr << "Error : " << filename << " when reading file" << std::endl;
        fclose(file);
        return (false);
    }

    cursor = 0;
    for (pixel = 0; pixel < _length; pixel += 4)
    {
        for (cursor = cursor; cursor < infoHeader.biSizeImage; cursor += 3)
        {
            ((unsigned char *)_data)[pixel] = buffer[cursor + 2];
            ((unsigned char *)_data)[pixel + 1] = buffer[cursor + 1];
            ((unsigned char *)_data)[pixel + 2] = buffer[cursor];
            ((unsigned char *)_data)[pixel + 3] = 1.0f;
        }
    }

    delete buffer;

    fclose(file);

    return (true);
}