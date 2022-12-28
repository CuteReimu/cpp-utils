#pragma once
#include <windows.h>
#include <string>
using std::string;
namespace Robot{
	class Keyboard
	{
	public:
		//发送字符串（字符串）
		void PressStr(const string &str);
		//按键（虚拟键值）
		void PressKey(BYTE bVk);
		//按键（虚拟键值1，虚拟键值2）
		void PressKey(BYTE bVk1, BYTE bVk2);
		//按键（虚拟键值1，虚拟键值2，虚拟键值3）
		void PressKey(BYTE bVk1, BYTE bVk2, BYTE bVk3);
		//按下（虚拟键值）
		void KeyDown(BYTE bVk);
		//抬起（虚拟键值）
		void KeyUp(BYTE bVk);
	};
}
