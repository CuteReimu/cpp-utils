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
			throw ios::failure("���ļ�ʧ��");
		}
		ifs.read((LPSTR)&bmfHeader, sizeof(BITMAPFILEHEADER));
		ifs.read((LPSTR)&bi, sizeof(BITMAPINFOHEADER));
		if(bi.biBitCount != 32){
			throw ios::failure("Ŀǰδʵ�����ֹ���");
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
			throw ios::failure("���ļ�ʧ��");
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
 
		// �� ͼƬͷ(headers)�Ĵ�С, ����λͼ�Ĵ�С����������ļ��Ĵ�С
		dwSizeofDIB = dwBmpSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
 
		// ���� Offset ƫ����λͼ��λ(bitmap bits)ʵ�ʿ�ʼ�ĵط�
		bmfHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER);
 
		// �ļ���С
		bmfHeader.bfSize = dwSizeofDIB;
 
		// λͼ�� bfType �������ַ��� "BM"
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
		HDC hdcScreen = GetDC(NULL); // ȫ��ĻDC
		HDC hdcMemDC = CreateCompatibleDC(hdcScreen); // ���������ڴ�DC
		BITMAPINFOHEADER bi;
 
		if (!hdcMemDC)
		{
			cout<<"CreateCompatibleDC has failed"<<endl;
			DeleteObject(hbmScreen);
			DeleteObject(hdcMemDC);
			ReleaseDC(NULL, hdcScreen);
			throw -1;
		}
 
		// ͨ������DC ����һ������λͼ
		hbmScreen = CreateCompatibleBitmap(hdcScreen, rect.width, rect.height);
 
		if (!hbmScreen)
		{
			cout<<"CreateCompatibleBitmap Failed"<<endl;
			DeleteObject(hbmScreen);
			DeleteObject(hdcMemDC);
			ReleaseDC(NULL, hdcScreen);
			throw -1;
		}
 
		// ��λͼ�鴫�͵����Ǽ��ݵ��ڴ�DC��
		SelectObject(hdcMemDC, hbmScreen);
		if (!BitBlt(
			hdcMemDC,    // Ŀ��DC
			0, 0,        // Ŀ��DC�� x,y ����
			rect.width, rect.height, // Ŀ�� DC �Ŀ��
			hdcScreen,   // ��ԴDC
			rect.x, rect.y,        // ��ԴDC�� x,y ����
			SRCCOPY))    // ճ����ʽ
		{
			cout<<"BitBlt has failed"<<endl;
			DeleteObject(hbmScreen);
			DeleteObject(hdcMemDC);
			ReleaseDC(NULL, hdcScreen);
			throw -1;
		}
 
		// ��ȡλͼ��Ϣ������� bmpScreen ��
		GetObject(hbmScreen, sizeof(BITMAP), &bmpScreen);

		dwBmpSize = rect.width * rect.height * 8;
 
		// �� 32-bit Windows ϵͳ��, GlobalAlloc �� LocalAlloc ���� HeapAlloc ��װ����
		// handle ָ�����Ĭ�ϵĶ�. ���Կ����� HeapAlloc Ҫ��
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
		// ��ȡ����λͼ��λ���ҿ��������һ�� lpbitmap ��.
		GetDIBits(
			hdcScreen,  // �豸�������
			hbmScreen,  // λͼ���
			0,          // ָ�������ĵ�һ��ɨ����
			(UINT)bmpScreen.bmHeight, // ָ��������ɨ������
			lpbitmap,   // ָ����������λͼ���ݵĻ�������ָ��
			(BITMAPINFO *)&bi, // �ýṹ�屣��λͼ�����ݸ�ʽ
			DIB_RGB_COLORS // ��ɫ���ɺ졢�̡�����RGB������ֱ��ֵ����
		);

		Image *img = new Image(lpbitmap, rect.width, rect.height);

		// �������ڴ沢�ͷ�
		GlobalUnlock(hDIB);
		GlobalFree(hDIB);

		// ������Դ
		return *img;
	}
}
