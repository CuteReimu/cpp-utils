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

	  //设置字符串内容
	  const SendString& operator = (const WCHAR *wszText);
	  //设置字符串内容
	  const SendString& operator = (const CHAR *szText);
	  //追加字符串内容
	  const SendString& operator += (const WCHAR *wszText);
	  //追加字符串内容
	  const SendString& operator += (const CHAR *szText);

	  /*模拟输入字符串，nTimes为重复模拟输入的次数
	  返回值：
		-1	出错，对象中保存的待输入字符串为空，不能模拟输入
		-2	出错，nTimes参数必须大于等于1
		-3	模拟失败，检查是否有360之类的软件拦截
		>0	模拟成功*/
	  INT Send(INT nTimes = 1);

	private:
	  //保存字符串内容
	  std::wstring m_strText;

	  //CHAR型字符串转换为wstring
	  std::wstring szTOwstr(const CHAR *szText);

	  //对于连续相同的字符，采取分隔处理
	  INPUT* Divide(UINT *nIput)const;

	  //对于没有连续相同的字符，采取不分隔处理
	  INPUT* NoDivede(UINT *nIput)const;
	  //nIput用于接收返回值，为返回的INPUT结构数组元素个数

	  //检查是否有连续相同的字符
	  bool CheckString()const;
  
	  //设置字符串内容
	  void SetString(const WCHAR *wszText);
	  //设置字符串内容
	  void SetString(const CHAR *szText);

	  //追加字符串内容
	  void AddString(const WCHAR *wszText);
	  //追加字符串内容
	  void AddString(const CHAR *szText);
	};
}
