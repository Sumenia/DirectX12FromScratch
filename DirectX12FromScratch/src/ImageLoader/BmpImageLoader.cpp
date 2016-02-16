#include <iostream>
#include "ImageLoader/BmpImageLoader.h"

BmpImageLoader::BmpImageLoader() : ImageLoader(EImageFormat::Type::BMP) {}

BmpImageLoader::~BmpImageLoader() {}

bool					BmpImageLoader::isBmp(FILE *file)
{
	BITMAPFILEHEADER	fileHeader;

	fread(&fileHeader, sizeof(BITMAPFILEHEADER), 1, file);

	if (fileHeader.bfType == 0x4D42)
		return (true);

	return (false);
}

bool                    BmpImageLoader::loadFromFile(FILE *file)
{
    BITMAPFILEHEADER    fileHeader;
    BITMAPINFOHEADER    infoHeader;
    unsigned char       *buffer;
    int                 cursor;
    int                 pixel;

	rewind(file);
	fread(&fileHeader, sizeof(BITMAPFILEHEADER), 1, file);
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
        std::cerr << "Error when reading file" << std::endl;
        fclose(file);
        return (false);
    }

	pixel = 0;
    cursor = 0;
	while (cursor < infoHeader.biSizeImage)
	{
		((unsigned char *)_data)[pixel] = buffer[cursor + 2];
		((unsigned char *)_data)[pixel + 1] = buffer[cursor + 1];
		((unsigned char *)_data)[pixel + 2] = buffer[cursor];
		((unsigned char *)_data)[pixel + 3] = 1.0f;
		pixel += 4;
		cursor += 3;
	}

    delete buffer;
    fclose(file);

    return (true);
}