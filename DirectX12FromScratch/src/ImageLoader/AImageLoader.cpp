#include "ImageLoader/AImageLoader.h"



AImageLoader::AImageLoader(EImageFormat::Type format):	 _data(NULL),
											_length(0),
											_format(format),
											_rgbFormat(ERGBFormat::Type::INVALID),
											_bitDepth(0),
											_width(0),
											_height(0) {}


AImageLoader::~AImageLoader()
{
	if (_data)
		delete _data;
}

void							*AImageLoader::getData() const
{
	return _data;
}

int								AImageLoader::getDataLength() const
{
	return _length;
}

int8_t							AImageLoader::getBitpDepth() const
{
	return _bitDepth;
}

int8_t							AImageLoader::getWidth() const
{
	return _width;
}

int8_t							AImageLoader::getHeight() const
{
	return _height;
}

EImageFormat::Type				AImageLoader::getFormat() const
{
	return _format;
}

ERGBFormat::Type				AImageLoader::getRGBFormat() const
{
	return _rgbFormat;
}