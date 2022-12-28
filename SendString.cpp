#include "stdafx.h"
#include "SendString.h"
#include <stdlib.h>
#include <locale.h>
#include <list>
namespace Robot{

	SendString::SendString()
	{
	  
	}

	SendString::SendString(const WCHAR *wszText)
	{
	  m_strText = wszText;
	}

	SendString::SendString(const CHAR *szText)
	{
	  m_strText = szTOwstr(szText);
	}

	void SendString::SetString(const WCHAR *wszText)
	{
	  m_strText = wszText;
	}

	void SendString::SetString(const CHAR *szText)
	{
	  m_strText = szTOwstr(szText);
	}

	void SendString::AddString(const WCHAR *wszText)
	{
	  m_strText.append(wszText);
	}

	void SendString::AddString(const CHAR *szText)
	{
	  m_strText.append(szTOwstr(szText));
	}


	SendString::operator std::wstring() const
	{
		return m_strText;
	}

	const SendString& SendString::operator = (const WCHAR *wszText)
	{
	  SetString(wszText);
	  return *this;
	}

	const SendString& SendString::operator = (const CHAR *szText)
	{
	  SetString(szText);
	  return *this;
	}

	const SendString& SendString::operator += (const WCHAR *wszText)
	{
	  AddString(wszText);
	  return *this;
	}

	const SendString& SendString::operator += (const CHAR *szText)
	{
	  AddString(szText);
	  return *this;
	}


	std::wstring SendString::szTOwstr(const CHAR *szText)
	{
	  //CHAR���ַ���ת��Ϊwstring
	  std::string curLocale = setlocale(LC_ALL, NULL);
	  setlocale(LC_ALL, "chs"); 
	  size_t _Dsize = strlen(szText) + 1;
	  WCHAR *wszText = new WCHAR[_Dsize];
	  wmemset(wszText,0,_Dsize);
	  size_t nValue;
	  mbstowcs_s(&nValue,wszText,_Dsize,szText,_TRUNCATE);
	  std::wstring wstrText = wszText;
	  delete []wszText;
	  setlocale(LC_ALL, curLocale.c_str());
	  return wstrText;
	} 

	INT SendString::Send(INT nTimes /* = 1 */)
	{	
	  //�������-1���ַ�������Ϊ��
	  if (m_strText.empty())
		return -1;
	  //�������-2���������������ڵ���1
	  if (nTimes < 1)
		return -2;
	  //aIputΪINPUT�ṹ���飬nIputΪINPUT�ṹ����Ԫ������
	  INPUT *aIput = NULL;
	  UINT nIput = 0;

	  if (CheckString())
	  {
		//��������ͬ���ַ�����ȡ�ָ�����
		aIput = Divide(&nIput);
	  }
	  else
	  {
		//û��������ͬ���ַ�����ȡ���ָ�����
		aIput = NoDivede(&nIput);
	  }
  
	  //��ʼ����ģ�ⰴ��
	  UINT nBeSend;
	  for (INT i=0;i<nTimes;i++)
	  {
		nBeSend = SendInput(nIput,aIput,sizeof(INPUT));
	  }

	  //���INPUT�ṹ���鲢�ͷ��ڴ�
	  delete [] aIput; 
  
	  if (nBeSend>0)
		//����ֵ����0��ʾÿ�γɹ�������ٸ��¼�
		return nBeSend;
	  else
		//�������-3��ģ��ʧ�ܣ��ɹ�ģ������0���ַ�
		return -3;
	}

	INPUT* SendString::Divide(UINT *nIput)
	const
	{
	  //nWordΪ�ַ�����
	  UINT nWord = (UINT)m_strText.length();

	  //����KEYBDINPUT�ṹ���飬��ÿ���ַ�����ȥ
	  KEYBDINPUT *aKey = new KEYBDINPUT[nWord];
	  //��սṹ��
	  memset(aKey,0,nWord*sizeof(KEYBDINPUT));
	  for (UINT i=0;i<nWord;i++)
	  {
		aKey[i].wScan = (WORD)m_strText[i];
		aKey[i].dwFlags = KEYEVENTF_UNICODE;
	  }

	  //��KEYBDINPUT�ṹ�������list������
	  std::list<KEYBDINPUT> Keylist;
	  for (UINT i=0;i<nWord;i++)
	  {
		Keylist.push_back(aKey[i]);
	  }

	  //ɾ��KEYBDINPUT�ṹ���鲢�ͷ���ռ�Ŀռ�
	  delete []aKey;

	  //׼����Ҫ���ķָ�����KEYBDINPUT�ṹ
	  KEYBDINPUT KEYdivision[4] = {NULL};
	  KEYdivision[0].wVk = VK_LEFT;
	  KEYdivision[1].wVk = VK_LEFT;
	  KEYdivision[1].dwFlags = KEYEVENTF_KEYUP;
	  KEYdivision[2].wVk = VK_RIGHT;
	  KEYdivision[3].wVk = VK_RIGHT;
	  KEYdivision[3].dwFlags = KEYEVENTF_KEYUP;

	  //��ʼ���������������KEYBDINPUT�ṹ������ַ���ͬ�����ָ�����
	  //��ȡ��һ��KEYBDINPUT�ṹ���ַ���������ָ��ڶ����ṹ
	  std::list<KEYBDINPUT>::iterator KeyIterator;
	  WORD word = Keylist.front().wScan;
	  KeyIterator = Keylist.begin();
	  KeyIterator++;
	  //ѭ���ԱȲ��Ҵ�����ͬ�ַ��Ľṹ
	  for(UINT i=0;i<nWord-1;i++)
	  {
		if (word == KeyIterator->wScan)
		{
		  //�������ڵĴ�����ͬ�ַ��Ľṹ������ָ�����
		  for (int i=0;i<4;i++)
		  {
			Keylist.insert(KeyIterator,KEYdivision[i]);
		  }
		}
		word = KeyIterator->wScan;
		KeyIterator++;
	  }

	  //����INPUT�ṹ����
	  UINT nPut = (UINT)Keylist.size();
	  INPUT *aPut = new INPUT[nPut];
	  //��սṹ��
	  memset(aPut,0,nPut*sizeof(INPUT));
	  //KEYBDINPUT�ṹ���list������ָ���һ��Ԫ��׼��ѭ�����INPUT�ṹ
	  KeyIterator = Keylist.begin();
	  for (UINT i=0;i<nPut;i++)
	  {
		aPut[i].type = INPUT_KEYBOARD;
		aPut[i].ki = *KeyIterator;
		KeyIterator++;
	  }

	  //����INPUT�ṹ����
	  *nIput = nPut;
	  //����INPUT�ṹָ�룬ʹ�����ע��delete
	  return aPut;
	}

	INPUT* SendString::NoDivede(UINT *nIput)
	const
	{
	  //nWordΪ�ַ�����
	  UINT nWord = (UINT)m_strText.length();

	  //����KEYBDINPUT�ṹ���飬��ÿ���ַ�����ȥ
	  KEYBDINPUT *aKey = new KEYBDINPUT[nWord];
	  //��սṹ��
	  memset(aKey,0,nWord*sizeof(KEYBDINPUT));
	  for (UINT i=0;i<nWord;i++)
	  {
		aKey[i].wScan = (WORD)m_strText[i];
		aKey[i].dwFlags = KEYEVENTF_UNICODE;
	  }

	  //����INPUT�ṹ����
	   INPUT *aPut = new INPUT[nWord];
	  memset(aPut,0,nWord*sizeof(INPUT));
	  for (UINT i=0;i<nWord;i++)
	  {
		aPut[i].type = INPUT_KEYBOARD;
		aPut[i].ki = aKey[i];
	  }

	  //ɾ��KEYBDINPUT�ṹ���鲢�ͷ���ռ�Ŀռ�
	  delete []aKey;
	  //����INPUT�ṹ����
	  *nIput = nWord;
	  //����INPUT�ṹָ�룬ʹ�����ע��delete
	  return aPut;
	}

	bool SendString::CheckString()
	const
	{
	  //����Ƿ������ڵ���ͬ���ַ�
	  WCHAR wch = m_strText[0];
	  UINT nWord = (UINT)m_strText.length();
	  for (UINT i=1;i<nWord;i++)
	  {
		if (wch == m_strText[i])
		{
		  return true;
		}
		wch = m_strText[i];
	  }
	  return false;
	}

}
