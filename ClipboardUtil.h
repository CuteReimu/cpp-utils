#pragma once
#include <string>
#include <windows.h>
using std::string;
namespace MyUtil{
	//获取剪贴板中的文字
	string getSysClipboardText();
	//将文字粘贴到剪贴板中
	void setSysClipboardText(const string &str);
}
