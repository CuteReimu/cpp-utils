#pragma once
#include <windows.h>
#include <string>
using std::string;
namespace Robot{
	class Keyboard
	{
	public:
		//�����ַ������ַ�����
		void PressStr(const string &str);
		//�����������ֵ��
		void PressKey(BYTE bVk);
		//�����������ֵ1�������ֵ2��
		void PressKey(BYTE bVk1, BYTE bVk2);
		//�����������ֵ1�������ֵ2�������ֵ3��
		void PressKey(BYTE bVk1, BYTE bVk2, BYTE bVk3);
		//���£������ֵ��
		void KeyDown(BYTE bVk);
		//̧�������ֵ��
		void KeyUp(BYTE bVk);
	};
}
