#include "stdafx.h"
#include "ImageUtil.h"
#include <cassert>
using namespace std;
namespace MyUtil{
	ImageFile::ImageFile(const string &fileName):File(fileName)
	{
	}
	Rect::Rect(int x, int y, int width, int height){
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
	}
	Color::Color(char A, char R, char G, char B){
		this->A = A;
		this->R = R;
		this->G = G;
		this->B = B;
	}
	Color::Color(int argb){
		this->A = argb>>24;
		this->R = argb>>16;
		this->G = argb>>8;
		this->B = argb;
	}
	bool Color::operator==(const Color &aColor) const{
		if(A!=aColor.A) return false;
		if(R!=aColor.R) return false;
		if(G!=aColor.G) return false;
		if(B!=aColor.B) return false;
		return true;
	}
	Color::operator int() const{
		return (A<<24) | (R<<16) | (G<<8) | B;
	}
	Image::Image(int width, int height){
		this->width = width;
		this->height = height;
		this->data = new int[width*height];
	}
	Image::Image(const void *data, int width, int height){
		this->width = width;
		this->height = height;
		this->data = new int[width*height];
		for(int j=0; j<height; j++)
			for(int i=0; i<width; i++)
				this->data[i+j*width] = (static_cast<const int *>(data))[i+j*width];
	}
	Image::Image(const string &fileName){
		BITMAPFILEHEADER bmfHeader;
		BITMAPINFOHEADER bi;
		ifstream ifs(fileName, ios::binary);
		if(ifs.fail()){
			throw ios::failure("打开文件失败");
		}
		ifs.read((LPSTR)&bmfHeader, sizeof(BITMAPFILEHEADER));
		ifs.read((LPSTR)&bi, sizeof(BITMAPINFOHEADER));
		if(bi.biBitCount != 32){
			throw ios::failure("目前未实现这种功能");
		}
		width = bi.biWidth;
		height = bi.biHeight;
		data = new int[width * height];
		ifs.read((LPSTR)data, bmfHeader.bfSize);
		ifs.close();
	}
	Image::~Image(){
		delete[] this->data;
	}
	
	bool Image::operator ==(const Image &aImg) const{
		if(height != aImg.height || width != aImg.width)
			return false;
		for(int j=0; j<height; j++)
			for(int i=0; i<width; i++)
				if(this->data[i+j*width] != aImg.data[i+j*width])
					return false;
		return true;
	}

	bool Image::operator !=(const Image &aImg) const{
		return !(*this==aImg);
	}

	Image &Image::subImage(const Rect &rect) const{
		Image *subImg = new Image(rect.width, rect.height);
		for(int j = 0; j<rect.height; j++){
			for(int i = 0; i<rect.width; i++){
				assert(i+j*subImg->width < subImg->width * subImg->height);
				if(i+rect.x>width || j+rect.y>height)
					subImg->data[i+j*subImg->width] = 0xff000000;
				else
					subImg->data[i+j*subImg->width] = data[i+rect.x+(j+rect.y)*width];
			}
		}
		return *subImg;
	}
	int Image::getWidth() const{
		return width;
	}
	int Image::getHeight() const{
		return height;
	}
	void Image::setPixel(int x, int y, const Color &clr){
		this->data[x+y*width] = clr;
	}
	Color Image::getPixel(int x, int y) const{
		Color clr(data[x+y*width]);
		return clr;
	}
	POINT Image::search(const Image &aImg, const Rect &area) const{
		for(int j=0; j<area.height - aImg.height + 1; j++){
			for(int i=0; i<area.width - aImg.width + 1; i++){
				Rect rect(i+area.x, j+area.y, aImg.width, aImg.height);
				Image &sub = this->subImage(rect);
				if(sub == aImg){
					delete &sub;
					POINT pt;
					pt.x=rect.x;
					pt.y=rect.y;
					return pt;
				} else
					delete &sub;
			}
		}
		POINT pt;
		pt.x=-1;
		pt.y=-1;
		return pt;
	}
	void Image::save(const string &fileName){
		ofstream ofs(fileName, ios::binary);
		if(ofs.fail()){
			throw ios::failure("打开文件失败");
		}

		DWORD dwSizeofDIB;
		DWORD dwBmpSize;
		BITMAPFILEHEADER bmfHeader;
		BITMAPINFOHEADER bi;
		bi.biSize = sizeof(BITMAPINFOHEADER);
		bi.biWidth = this->width;
		bi.biHeight = this->height;
		bi.biPlanes = 1;
		bi.biBitCount = 32;
		bi.biCompression = BI_RGB;
		bi.biSizeImage = 0;
		bi.biXPelsPerMeter = 0;
		bi.biYPelsPerMeter = 0;
		bi.biClrUsed = 0;
		bi.biClrImportant = 0;
		int i=0;

		dwBmpSize = bi.biWidth * bi.biHeight * (bi.biBitCount/8);
 
		// 将 图片头(headers)的大小, 加上位图的大小来获得整个文件的大小
		dwSizeofDIB = dwBmpSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
 
		// 设置 Offset 偏移至位图的位(bitmap bits)实际开始的地方
		bmfHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER);
 
		// 文件大小
		bmfHeader.bfSize = dwSizeofDIB;
 
		// 位图的 bfType 必须是字符串 "BM"
		bmfHeader.bfType = 0x4D42; //BM
 
		LPSTR p = (LPSTR)&bmfHeader;
		ofs.write(p, sizeof(BITMAPFILEHEADER));
		p = (LPSTR)&bi;
		ofs.write(p, sizeof(BITMAPINFOHEADER));
		ofs.write((LPSTR)this->data, dwBmpSize);
		ofs.close();
	}
	Image &CaptureImage(const Rect &rect)
	{
		HANDLE hDIB;
		HBITMAP hbmScreen = NULL;
		BITMAP bmpScreen;
		DWORD dwBmpSize;
		CHAR *lpbitmap;
		HDC hdcScreen = GetDC(NULL); // 全屏幕DC
		HDC hdcMemDC = CreateCompatibleDC(hdcScreen); // 创建兼容内存DC
		BITMAPINFOHEADER bi;
 
		if (!hdcMemDC)
		{
			cout<<"CreateCompatibleDC has failed"<<endl;
			DeleteObject(hbmScreen);
			DeleteObject(hdcMemDC);
			ReleaseDC(NULL, hdcScreen);
			throw -1;
		}
 
		// 通过窗口DC 创建一个兼容位图
		hbmScreen = CreateCompatibleBitmap(hdcScreen, rect.width, rect.height);
 
		if (!hbmScreen)
		{
			cout<<"CreateCompatibleBitmap Failed"<<endl;
			DeleteObject(hbmScreen);
			DeleteObject(hdcMemDC);
			ReleaseDC(NULL, hdcScreen);
			throw -1;
		}
 
		// 将位图块传送到我们兼容的内存DC中
		SelectObject(hdcMemDC, hbmScreen);
		if (!BitBlt(
			hdcMemDC,    // 目的DC
			0, 0,        // 目的DC的 x,y 坐标
			rect.width, rect.height, // 目的 DC 的宽高
			hdcScreen,   // 来源DC
			rect.x, rect.y,        // 来源DC的 x,y 坐标
			SRCCOPY))    // 粘贴方式
		{
			cout<<"BitBlt has failed"<<endl;
			DeleteObject(hbmScreen);
			DeleteObject(hdcMemDC);
			ReleaseDC(NULL, hdcScreen);
			throw -1;
		}
 
		// 获取位图信息并存放在 bmpScreen 中
		GetObject(hbmScreen, sizeof(BITMAP), &bmpScreen);

		dwBmpSize = rect.width * rect.height * 8;
 
		// 在 32-bit Windows 系统上, GlobalAlloc 和 LocalAlloc 是由 HeapAlloc 封装来的
		// handle 指向进程默认的堆. 所以开销比 HeapAlloc 要大
		hDIB = GlobalAlloc(GHND, dwBmpSize);
		lpbitmap = (char *)GlobalLock(hDIB);

		bi.biSize = sizeof(BITMAPINFOHEADER);
		bi.biWidth = rect.width;
		bi.biHeight = rect.height;
		bi.biPlanes = 1;
		bi.biBitCount = 32;
		bi.biCompression = BI_RGB;
		bi.biSizeImage = 0;
		bi.biXPelsPerMeter = 0;
		bi.biYPelsPerMeter = 0;
		bi.biClrUsed = 0;
		bi.biClrImportant = 0;
		// 获取兼容位图的位并且拷贝结果到一个 lpbitmap 中.
		GetDIBits(
			hdcScreen,  // 设备环境句柄
			hbmScreen,  // 位图句柄
			0,          // 指定检索的第一个扫描线
			(UINT)bmpScreen.bmHeight, // 指定检索的扫描线数
			lpbitmap,   // 指向用来检索位图数据的缓冲区的指针
			(BITMAPINFO *)&bi, // 该结构体保存位图的数据格式
			DIB_RGB_COLORS // 颜色表由红、绿、蓝（RGB）三个直接值构成
		);

		Image *img = new Image(lpbitmap, rect.width, rect.height);

		// 解锁堆内存并释放
		GlobalUnlock(hDIB);
		GlobalFree(hDIB);

		// 清理资源
		return *img;
	}
}
