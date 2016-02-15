#include "ImageLoader/ImageLoader.h"



ImageLoader::ImageLoader(EImageFormat::Type format):	 _data(NULL),
											_length(0),
											_format(format),
											_rgbFormat(ERGBFormat::Type::INVALID),
											_bitDepth(0),
											_width(0),
											_height(0) {}


ImageLoader::~ImageLoader()
{
	if (_data)
		delete _data;
}

void							*ImageLoader::getData() const
{
	return _data;
}

int								ImageLoader::getDataLength() const
{
	return _length;
}

int8_t							ImageLoader::getBitpDepth() const
{
	return _bitDepth;
}

unsigned int					ImageLoader::getWidth() const
{
	return _width;
}

unsigned int					ImageLoader::getHeight() const
{
	return _height;
}

EImageFormat::Type				ImageLoader::getFormat() const
{
	return _format;
}

ERGBFormat::Type				ImageLoader::getRGBFormat() const
{
	return _rgbFormat;
}