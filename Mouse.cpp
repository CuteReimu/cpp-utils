#include "Mouse.h"
namespace Robot{

	void Mouse::Move(int x,int y) const
	{
		::SetCursorPos(x,y);
	}

	void Mouse::RelativeMove(int cx,int cy) const
	{
		POINT point;
		::GetCursorPos(&point);
		::SetCursorPos(point.x+cx,point.y+cy);
	}

	POINT Mouse::getPos() const
	{
		POINT point;
		::GetCursorPos(&point);
		return point;
	}

	void Mouse::SavePos()
	{
		::GetCursorPos(&this->point);
	}

	void Mouse::RestorePos() const
	{
		::SetCursorPos(this->point.x,this->point.y);
	}

	void Mouse::Lock() const
	{
		POINT    pt;
		RECT    rt;

		::GetCursorPos(&pt);
		rt.left=rt.right=pt.x;
		rt.top=rt.bottom=pt.y;
		rt.right++;
		rt.bottom++;
		::ClipCursor(&rt);
	}

	void Mouse::Unlock() const
	{
		::ClipCursor(NULL);
	}

	void Mouse::LBClick() const
	{
		POINT point;
		::GetCursorPos(&point);
		::mouse_event(MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_LEFTUP,point.x,point.y,0,0);
	}

	void Mouse::LBDbClick() const
	{
		POINT point;
		::GetCursorPos(&point);
		::mouse_event(MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_LEFTUP,point.x,point.y,0,0);
		::mouse_event(MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_LEFTUP,point.x,point.y,0,0);
	}

	void Mouse::LBDown() const
	{
		POINT point;
		::GetCursorPos(&point);
		::mouse_event(MOUSEEVENTF_LEFTDOWN,point.x,point.y,0,0);
	}

	void Mouse::LBUp() const
	{
		POINT point;
		::GetCursorPos(&point);
		::mouse_event(MOUSEEVENTF_LEFTUP,point.x,point.y,0,0);
	}

	void Mouse::RBClick() const
	{
		POINT point;
		::GetCursorPos(&point);
		::mouse_event(MOUSEEVENTF_RIGHTDOWN|MOUSEEVENTF_RIGHTUP,point.x,point.y,0,0);
	}

	void Mouse::RBDbClick() const
	{
		POINT point;
		::GetCursorPos(&point);
		::mouse_event(MOUSEEVENTF_RIGHTDOWN|MOUSEEVENTF_RIGHTUP,point.x,point.y,0,0);
		::mouse_event(MOUSEEVENTF_RIGHTDOWN|MOUSEEVENTF_RIGHTUP,point.x,point.y,0,0);
	}

	void Mouse::RBDown() const
	{
		POINT point;
		::GetCursorPos(&point);
		::mouse_event(MOUSEEVENTF_RIGHTDOWN,point.x,point.y,0,0);
	}

	void Mouse::RBUp() const
	{
		POINT point;
		::GetCursorPos(&point);
		::mouse_event(MOUSEEVENTF_RIGHTUP,point.x,point.y,0,0);
	}

	void Mouse::MBClick() const
	{
		POINT point;
		::GetCursorPos(&point);
		::mouse_event(MOUSEEVENTF_MIDDLEDOWN|MOUSEEVENTF_MIDDLEUP,point.x,point.y,0,0);
	}

	void Mouse::MBDbClick() const
	{
		POINT point;
		::GetCursorPos(&point);
		::mouse_event(MOUSEEVENTF_MIDDLEDOWN|MOUSEEVENTF_MIDDLEUP,point.x,point.y,0,0);
		::mouse_event(MOUSEEVENTF_MIDDLEDOWN|MOUSEEVENTF_MIDDLEUP,point.x,point.y,0,0);
	}

	void Mouse::MBDown() const
	{
		POINT point;
		::GetCursorPos(&point);
		::mouse_event(MOUSEEVENTF_MIDDLEDOWN,point.x,point.y,0,0);
	}

	void Mouse::MBUp() const
	{
		POINT point;
		::GetCursorPos(&point);
		::mouse_event(MOUSEEVENTF_MIDDLEUP,point.x,point.y,0,0);
	}

	void Mouse::MBRoll(int ch) const
	{
		POINT point;
		::GetCursorPos(&point);
		::mouse_event(MOUSEEVENTF_WHEEL,point.x,point.y,ch,0);
	}

}