#pragma once

# include <string>
# include "stdint.h"

class IImageLoader
{
public:
	enum Format
	{
		PNG = 0,
		JPG = 1,
		BMP = 2
	};

	enum RGBFormat
	{
		RGB = 0,
		RGBA = 1,
		GRAY = 2,
		GRAY_ALPHA = 3
	};

public:
	IImageLoader(Format format);
	virtual ~IImageLoader();
	void			*getData() const;
	int				getDataLength() const;
	int8_t			getBitpDepth() const;
	int8_t			getWidth() const;
	int8_t			getHeight() const;
	Format			getFormat() const;
	RGBFormat		getRGBFormat() const;
	virtual bool	loadFromFile(const std::string &filename) = 0;

protected:
	void			*_data;
	size_t			_length;
	Format			_format;
	RGBFormat		_rgbFormat;
	int8_t			_bitDepth;
	int32_t			_width;
	int32_t			_height;
};