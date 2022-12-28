#pragma once
#ifndef _WIN32_WINNT		
#define _WIN32_WINNT 0x0501
#endif
#include <windows.h>
#include <string>
namespace Robot{
	class SendString 
	{
	public:
	  SendString();
	  SendString(const WCHAR *wszText);
	  SendString(const CHAR *szText);

	  operator std::wstring() const;

	  //�����ַ�������
	  const SendString& operator = (const WCHAR *wszText);
	  //�����ַ�������
	  const SendString& operator = (const CHAR *szText);
	  //׷���ַ�������
	  const SendString& operator += (const WCHAR *wszText);
	  //׷���ַ�������
	  const SendString& operator += (const CHAR *szText);

	  /*ģ�������ַ�����nTimesΪ�ظ�ģ������Ĵ���
	  ����ֵ��
		-1	���������б���Ĵ������ַ���Ϊ�գ�����ģ������
		-2	����nTimes����������ڵ���1
		-3	ģ��ʧ�ܣ�����Ƿ���360֮����������
		>0	ģ��ɹ�*/
	  INT Send(INT nTimes = 1);

	private:
	  //�����ַ�������
	  std::wstring m_strText;

	  //CHAR���ַ���ת��Ϊwstring
	  std::wstring szTOwstr(const CHAR *szText);

	  //����������ͬ���ַ�����ȡ�ָ�����
	  INPUT* Divide(UINT *nIput)const;

	  //����û��������ͬ���ַ�����ȡ���ָ�����
	  INPUT* NoDivede(UINT *nIput)const;
	  //nIput���ڽ��շ���ֵ��Ϊ���ص�INPUT�ṹ����Ԫ�ظ���

	  //����Ƿ���������ͬ���ַ�
	  bool CheckString()const;
  
	  //�����ַ�������
	  void SetString(const WCHAR *wszText);
	  //�����ַ�������
	  void SetString(const CHAR *szText);

	  //׷���ַ�������
	  void AddString(const WCHAR *wszText);
	  //׷���ַ�������
	  void AddString(const CHAR *szText);
	};
}
