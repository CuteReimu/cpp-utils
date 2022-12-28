#pragma once
#include <windows.h>
namespace Robot{
	class Mouse
	{
	private:
		POINT point;
	public:
		//�ƶ���굽����λ�ã�X���꣬Y���꣩
		void Move(int x,int y) const;
		//�ƶ���굽���λ�ã�Xλ�ƣ�Yλ�ƣ�
		void RelativeMove(int cx,int cy) const;
		//��ȡ��ǰλ��
		POINT getPos() const;
		//���浱ǰλ��
		void SavePos();
		//�ָ����λ��
		void RestorePos() const;
		//�������
		void Lock() const;
		//�������
		void Unlock() const;
		//�������
		void LBClick() const;
		//���˫��
		void LBDbClick() const;
		//�������
		void LBDown() const;
		//���̧��
		void LBUp() const;
		//�Ҽ�����
		void RBClick() const;
		//�Ҽ�˫��
		void RBDbClick() const;
		//�Ҽ�����
		void RBDown() const;
		//�Ҽ�̧��
		void RBUp() const;
		//�м�����
		void MBClick() const;
		//�м�˫��
		void MBDbClick() const;
		//�м�����
		void MBDown() const;
		//�м�̧��
		void MBUp() const;
		//�м�����������λ�ƣ�
		void MBRoll(int ch) const;
	};
}
