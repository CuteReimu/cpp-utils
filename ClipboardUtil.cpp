#include "stdafx.h"
#include "ClipboardUtil.h"
using std::string;
namespace MyUtil{
	string getSysClipboardText(){
		string str;
		if (::OpenClipboard(NULL))//打开剪贴板  
		{
			if (IsClipboardFormatAvailable(CF_TEXT))//判断格式是否是我们所需要  
			{  
				HANDLE hClip;  
				char* pBuf;

				//读取数据  
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
			EmptyClipboard();//清空剪贴板  
   
			//写入数据  
			hClip = ::GlobalAlloc(GMEM_MOVEABLE, str.length()+1);  
			pBuf = (char*)::GlobalLock(hClip);
			for(int i=0; str[i]!='\0'; i++){
				pBuf[i] = str[i];
			}
			::GlobalUnlock(hClip);//解锁  
			::SetClipboardData(CF_TEXT, hClip);//设置格式  
   
			//关闭剪贴板  
			::CloseClipboard();  
		}  
	}
}