#pragma once
#include <windows.h>
namespace Robot{
	class Mouse
	{
	private:
		POINT point;
	public:
		//移动鼠标到绝对位置（X坐标，Y坐标）
		void Move(int x,int y) const;
		//移动鼠标到相对位置（X位移，Y位移）
		void RelativeMove(int cx,int cy) const;
		//获取当前位置
		POINT getPos() const;
		//保存当前位置
		void SavePos();
		//恢复鼠标位置
		void RestorePos() const;
		//锁定鼠标
		void Lock() const;
		//解锁鼠标
		void Unlock() const;
		//左键单击
		void LBClick() const;
		//左键双击
		void LBDbClick() const;
		//左键按下
		void LBDown() const;
		//左键抬起
		void LBUp() const;
		//右键单击
		void RBClick() const;
		//右键双击
		void RBDbClick() const;
		//右键按下
		void RBDown() const;
		//右键抬起
		void RBUp() const;
		//中键单击
		void MBClick() const;
		//中键双击
		void MBDbClick() const;
		//中键按下
		void MBDown() const;
		//中键抬起
		void MBUp() const;
		//中键滚动（滚动位移）
		void MBRoll(int ch) const;
	};
}
