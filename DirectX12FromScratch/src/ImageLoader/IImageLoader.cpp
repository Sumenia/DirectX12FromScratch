#include "ImageLoader/IImageLoader.h"



IImageLoader::IImageLoader(Format format): _format(format) {}


IImageLoader::~IImageLoader() {}

void							*IImageLoader::getData() const
{
	return _data;
}

int								IImageLoader::getDataLength() const
{
	return _length;
}

int8_t							IImageLoader::getBitpDepth() const
{
	return _bitDepth;
}

int8_t							IImageLoader::getWidth() const
{
	return _width;
}

int8_t							IImageLoader::getHeight() const
{
	return _height;
}

IImageLoader::Format			IImageLoader::getFormat() const
{
	return _format;
}

IImageLoader::RGBFormat			IImageLoader::getRGBFormat() const
{
	return _rgbFormat;
}