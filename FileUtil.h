#pragma once
#include <string>
#include <vector>
#include <io.h>
using std::string;
using std::vector;
namespace MyUtil{
	//��ȡĳĿ¼������ָ����չ���ļ�
	void getFiles(const string &path, const string &exd, vector<string> &files);
	class File
	{
	public:
		File(const string &fileName);
		virtual ~File(void);
	protected:
		string fileName;
	};
}
