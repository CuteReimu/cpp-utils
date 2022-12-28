#pragma once
#include <string>
#include <windows.h>
#include <iostream>
#include <fstream>
#include "FileUtil.h"
using std::string;
namespace MyUtil{
	class ImageFile:public File
	{
	public:
		ImageFile(const string &fileName);
	};
	class Rect{
	public:
		Rect(int x, int y, int width, int height);
		int x;
		int y;
		int width;
		int height;
	};
	class Color{
	public:
		char A;
		char R;
		char G;
		char B;
		Color(char A, char R, char G, char B);
		Color(int rgba);
		bool operator ==(const Color &aColor) const;
		operator int() const;
	};
	class Image{
	private:
		int *data;
		int width;
		int height;
	public:
		Image(int width, int height);
		Image(const void *data, int width, int height);
		Image(const string &fileName);
		virtual ~Image();
		bool operator ==(const Image &aImg) const;
		bool operator !=(const Image &aImg) const;
		Image &subImage(const Rect &rect) const;
		int getWidth() const;
		int getHeight() const;
		void setPixel(int x, int y, const Color &clr);
		Color getPixel(int x, int y) const;
		POINT search(const Image &aImg, const Rect &area) const;
		void save(const string &fileName);
	};

	/*
	 GDI 截屏函数
	 参数 rect 截图区域
	 参数 dirPath 截图存放目录
	 参数 filename 截图名称
	 */
	Image &CaptureImage(const Rect &rect);
}
