#pragma once
#include <string>
#include <windows.h>
using std::string;
namespace MyUtil{
	//��ȡ�������е�����
	string getSysClipboardText();
	//������ճ������������
	void setSysClipboardText(const string &str);
}
