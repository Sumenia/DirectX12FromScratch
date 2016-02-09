#pragma once

# include "IImageLoader.h"
# include "libPNG/lpng1621/png.h"

class PngImageLoader : public IImageLoader
{
public:
	PngImageLoader();
	~PngImageLoader();
	virtual bool	loadFromFile(const std::string &filename);

private:
	bool			writeToFile(const std::string &filename);
	void			setColorType(png_byte colorType, png_structp &png_ptr);
	bool			removeAlphaChannel(FILE *fp, png_infop &png_info, png_structp &png_ptr);
};