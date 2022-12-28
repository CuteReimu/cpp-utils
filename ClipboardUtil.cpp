#include "stdafx.h"
#include "ClipboardUtil.h"
using std::string;
namespace MyUtil{
	string getSysClipboardText(){
		string str;
		if (::OpenClipboard(NULL))//�򿪼�����  
		{
			if (IsClipboardFormatAvailable(CF_TEXT))//�жϸ�ʽ�Ƿ�����������Ҫ  
			{  
				HANDLE hClip;  
				char* pBuf;

				//��ȡ����  
				hClip=::GetClipboardData(CF_TEXT);  
				pBuf=(char*)::GlobalLock(hClip);
				str = pBuf;
				::GlobalUnlock(hClip);  
			}
			::CloseClipboard();  
		}
		return str;
	}

	void setSysClipboardText(const string &str){
		if(::OpenClipboard(NULL))  
		{  
			HANDLE hClip;  
			char* pBuf;  
			EmptyClipboard();//��ռ�����  
   
			//д������  
			hClip = ::GlobalAlloc(GMEM_MOVEABLE, str.length()+1);  
			pBuf = (char*)::GlobalLock(hClip);
			for(int i=0; str[i]!='\0'; i++){
				pBuf[i] = str[i];
			}
			::GlobalUnlock(hClip);//����  
			::SetClipboardData(CF_TEXT, hClip);//���ø�ʽ  
   
			//�رռ�����  
			::CloseClipboard();  
		}  
	}
}