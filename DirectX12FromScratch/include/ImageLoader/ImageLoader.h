#pragma once

# include <string>
# include "stdint.h"

namespace EImageFormat
{
	enum Type
	{
		INVALID = -1,
		PNG = 0,
		JPG = 1,
		BMP = 2
	};
}

namespace ERGBFormat
{
	enum Type
	{
		INVALID = -1,
		RGB = 0,
		RGBA = 1,
		GRAY = 2,
		GRAY_ALPHA = 3
	};
}

class ImageLoader
{
public:
	ImageLoader(EImageFormat::Type format);
	virtual ~ImageLoader();

	void					*getData() const;
	int						getDataLength() const;
	int8_t					getBitpDepth() const;
	int8_t					getWidth() const;
	int8_t					getHeight() const;
	EImageFormat::Type		getFormat() const;
	ERGBFormat::Type		getRGBFormat() const;
	virtual bool			loadFromFile(const std::string &filename) = 0;

protected:
	void					*_data;
	size_t					_length;
	EImageFormat::Type		_format;
	ERGBFormat::Type		_rgbFormat;
	int8_t					_bitDepth;
	int32_t					_width;
	int32_t					_height;
};