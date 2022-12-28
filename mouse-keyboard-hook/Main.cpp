#include <windows.h>
#include "Mouse.h"
#include "Keyboard.h"
#include "thread.h"
using namespace Robot;
using namespace ThreadAttach;
HWND hgWnd;
HHOOK myhook;
bool isEnd = true;
 
/**************************************************************** 
  WH_KEYBOARD hook procedure 
  �I�P���Ӵ������
 ****************************************************************/
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) 
{   
    PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;
    const char *info = NULL;
    char text[50], data[20];
 
    PAINTSTRUCT ps;
    HDC hdc;

    if (nCode >= 0)
    {
        if      (wParam == WM_KEYDOWN)      info = "��ͨ���I����";
        else if (wParam == WM_KEYUP)        info = "��ͨ���I̧��";
        else if (wParam == WM_SYSKEYDOWN)   info = "ϵ�y���I����";
        else if (wParam == WM_SYSKEYUP)     info = "ϵ�y���I̧��";
		if (p->vkCode == 'R') {
			if (wParam == WM_KEYDOWN) {
				isEnd = false;
			}
			if (wParam == WM_KEYUP) {
				isEnd = true;
			}
		}
 
        ZeroMemory(text, sizeof(text));
        ZeroMemory(data, sizeof(data));
        wsprintf(text, "%s - ������ [%04d], ɨ���� [%04d]  ", info, p->vkCode, p->scanCode);
        wsprintf(data, "���IĿ�y�飺 %c  ", p->vkCode);
 
        hdc = GetDC(hgWnd);         
        TextOut(hdc, 10, 10, text, strlen(text));
        TextOut(hdc, 10, 30, data, strlen(data));
        ReleaseDC(hgWnd,hdc);
    }
     
    return CallNextHookEx(myhook, nCode, wParam, lParam);
} 
/**************************************************************** 
  WH_KEYBOARD hook procedure 
  �I�P���Ӵ������
 ****************************************************************/
LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam) 
{   
    LPMSLLHOOKSTRUCT p = (LPMSLLHOOKSTRUCT)lParam;
    POINT   pt = p->pt;
    DWORD   mouseData = p->mouseData;
    const char *info = NULL;
    char text[60], pData[50], mData[50];
 
    PAINTSTRUCT ps;
    HDC hdc;
 
    if (nCode >= 0)
    {
        if   (wParam == WM_MOUSEMOVE)       info = "����ƶ���������";
        else if(wParam == WM_LBUTTONDOWN)   info = "��꡾���������";
        else if(wParam == WM_LBUTTONUP)     info = "��꡾�����̧��";
        else if(wParam == WM_LBUTTONDBLCLK) info = "��꡾�����˫��";
        else if(wParam == WM_RBUTTONDOWN)   info = "��꡾�Ҽ�������";
        else if(wParam == WM_RBUTTONUP)     info = "��꡾�Ҽ���̧��";
        else if(wParam == WM_RBUTTONDBLCLK) info = "��꡾�Ҽ���˫��";
        else if(wParam == WM_MBUTTONDOWN)   info = "��꡾���֡�����";
        else if(wParam == WM_MBUTTONUP)     info = "��꡾���֡�̧��";
        else if(wParam == WM_MBUTTONDBLCLK) info = "��꡾���֡�˫��";
        else if(wParam == WM_MOUSEWHEEL)    info = "��꡾���֡�����";

		if (wParam == WM_RBUTTONDOWN){
			isEnd = false;
		}
		if (wParam == WM_RBUTTONUP){
			isEnd = true;
		}
 
        ZeroMemory(text, sizeof(text));
        ZeroMemory(pData, sizeof(pData));
        ZeroMemory(mData, sizeof(mData));
 
        wsprintf( text, "��ǰ״̬�� %10s   ", info);
        wsprintf(pData, "0x%x - X: [%04d], Y: [%04d]  ", wParam, pt.x, pt.y);
        wsprintf(mData, "�������ݣ� %16u   ", mouseData);
 
        hdc = GetDC(hgWnd);         
        TextOut(hdc, 10, 10,  text, strlen(text));
        TextOut(hdc, 10, 30, pData, strlen(pData));
        TextOut(hdc, 10, 50, mData, strlen(mData));
        ReleaseDC(hgWnd,hdc);
    }
     
    return CallNextHookEx(myhook, nCode, wParam, lParam);
} 
 
// 5. ���ڹ��̴���
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{   
    hgWnd = hwnd;
 
    switch(msg)
    {
        case WM_CLOSE:
            DestroyWindow(hwnd);
			exit(0);
        break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

class myThread:public Runnable
{
public:
	virtual unsigned int _stdcall run(void *p) {
		boolean last = isEnd;
		Keyboard k;
		Mouse m;
		while (true) {
			if (!isEnd) {
				k.KeyDown('9');
				Sleep(20);
			}
			if (!isEnd) {
				k.KeyUp('9');
				Sleep(20);
			}
			if (!isEnd) {
				k.KeyDown('F');
				Sleep(20);
			}
			if (!isEnd) {
				k.KeyUp('F');
			}
			if (isEnd && !last) {
				k.KeyUp('F');
				k.KeyUp('9');
			}
			Sleep(20);
			last = isEnd;
		}
	}
};
 
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
{
	myThread mt;
	Thread t(mt);
	t.start();

    WNDCLASSEX wc;  // http://baike.baidu.com/view/1750396.htm
    HWND hwnd;
    MSG Msg;
    char text[30];
 
    const char szClassName[] = "myWindowClass";
 
    // 1. ����ע�ᴰ�ڽṹ��
    wc.cbSize        = sizeof(WNDCLASSEX);              // ע�ᴰ�ڽṹ��Ĵ�С
    wc.style         = 0;                               // ���ڵ���ʽ
    wc.lpfnWndProc   = WndProc;                         // ָ�򴰿ڴ�����̵ĺ���ָ��
    wc.cbClsExtra    = 0;                               // ָ�������ڴ�����ṹ��ĸ����ֽ���
    wc.cbWndExtra    = 0;                               // ָ�������ڴ���������ĸ����ֽ���
    wc.hInstance     = hInstance;                       // ��ģ���ʵ�����
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION); // ͼ��ľ��
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);     // ���ľ��
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);        // ������ˢ�ľ��
    wc.lpszMenuName  = NULL;                            // ָ��˵���ָ��
    wc.lpszClassName = szClassName;                     // ָ�������Ƶ�ָ��
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION); // �ʹ����������Сͼ��
 
    // 2. ʹ�á����ڽṹ�塿ע�ᴰ��
    if(!RegisterClassEx(&wc))
    {
        MessageBox(NULL, TEXT("����ע��ʧ�ܣ�"), TEXT("����"), MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }
 
    // 3. ��������
    hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,       // ���ڵ���չ���
        szClassName,            // ָ��ע��������ָ��
        TEXT("���ڱ���"),       // ָ�򴰿����Ƶ�ָ��
        WS_OVERLAPPEDWINDOW,    // ���ڷ��
        CW_USEDEFAULT, CW_USEDEFAULT, 350, 200, // ���ڵ� x,y �����Լ����
        NULL,                   // �����ڵľ��
        NULL,                   // �˵��ľ��
        hInstance,              // Ӧ�ó���ʵ���ľ��
        NULL                    // ָ�򴰿ڵĴ�������
        );
 
    if(hwnd == NULL)
    {
        MessageBox(NULL, TEXT("���ڴ���ʧ��"), TEXT("����"),MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }
 
    // 4. ��ʾ����
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
 
    // �������ȫ�ּ���
    myhook = SetWindowsHookEx( 
        WH_MOUSE_LL,    // �������͡���꡿
        MouseProc,  // ������
        hInstance,      // ��ǰʵ�����
        0               // �������ھ��(NULLΪȫ�ּ���)
    ); 
 
    if(myhook == NULL)
    {       
        wsprintf(text, "���̼���ʧ�ܣ�error : %d \n", GetLastError());
        MessageBox(hwnd, text, TEXT("����"), MB_OK);
    }
 
 
    // 5. ��Ϣѭ��
    while(GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    return Msg.wParam;
}