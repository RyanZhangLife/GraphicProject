#include "Image.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace GT
{
	Image* Image::readFromFile(const char* _fileName) {
		int _picType = 0;
		int _width = 0;
		int _height = 0;

		// stbImage读取的图片是反着的,使用下面函数把图片反转
		stbi_set_flip_vertically_on_load(true);
		unsigned char* bits = stbi_load(_fileName, &_width, &_height, &_picType, STBI_rgb_alpha);
		
		for (int i = 0; i < _width * _height * 4; i+=4)
		{
			byte tmp = bits[i];
			bits[i] = bits[i + 2];
			bits[i + 2] = tmp;
		}
		
		Image* _image = new Image(_width, _height, bits);
		// 释放bits
		stbi_image_free(bits);

		return _image;
	}

	Image* Image::zoomImage(const Image* _image, float _zoomX, float _zoomY)
	{
		int _width = _image->getWidth() * _zoomX;
		int _height = _image->getHeight() * _zoomY;
		byte* _data = new byte[_width * _height * sizeof(RGBA)];
		Image* _resultImage = nullptr;

		for (int i = 0; i < _width; ++i)
		{
			for (int j = 0; j < _height; ++j)
			{
				int _imageX = (float)i / _zoomX;
				int _imageY = (float)j / _zoomY;

				_imageX = _imageX < _image->getWidth() ? _imageX : (_image->getWidth() - 1);
				_imageY = _imageY < _image->getHeight() ? _imageY : (_image->getHeight() - 1);

				RGBA _color = _image->getColor(_imageX, _imageY);
				memcpy(_data + (j * _width + i) * sizeof(RGBA), &_color, sizeof(RGBA));
			}
		}

		_resultImage = new Image(_width, _height, _data);

		delete _data;
		return _resultImage;
	}
}