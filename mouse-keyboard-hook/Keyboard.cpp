#include "Keyboard.h"
namespace Robot{
	void Keyboard::PressKey(BYTE bVk)
	{
		::keybd_event(bVk,0,0,0);
		::keybd_event(bVk,0,KEYEVENTF_KEYUP,0);
	}

	void Keyboard::PressKey(BYTE bVk1, BYTE bVk2)
	{
		::keybd_event(bVk1,0,0,0);
		::keybd_event(bVk2,0,0,0);
		::keybd_event(bVk2,0,KEYEVENTF_KEYUP,0);
		::keybd_event(bVk1,0,KEYEVENTF_KEYUP,0);
	}

	void Keyboard::PressKey(BYTE bVk1, BYTE bVk2, BYTE bVk3)
	{
		::keybd_event(bVk1,0,0,0);
		::keybd_event(bVk2,0,0,0);
		::keybd_event(bVk3,0,0,0);
		::keybd_event(bVk3,0,KEYEVENTF_KEYUP,0);
		::keybd_event(bVk2,0,KEYEVENTF_KEYUP,0);
		::keybd_event(bVk1,0,KEYEVENTF_KEYUP,0);
	}

	void Keyboard::KeyDown(BYTE bVk)
	{
		::keybd_event(bVk,0,0,0);
	}

	void Keyboard::KeyUp(BYTE bVk)
	{
		::keybd_event(bVk,0,KEYEVENTF_KEYUP,0);
	}

	void Keyboard::PressStr(const string &str)
	{
		for (unsigned i=0;i<str.length();i++)
		{
			if (str[i]>='a' && str[i]<='z')
				this->PressKey(str[i]-0x20);
			else if (str[i]>='A' && str[i]<='Z')
				this->PressKey(VK_SHIFT, str[i]);
			else if (str[i]>='0' && str[i]<='9')
				this->PressKey(str[i]+0x10);
			else
				this->PressKey(str[i]);
		}
	}
}
