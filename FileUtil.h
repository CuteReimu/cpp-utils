#pragma once
#include <string>
#include <vector>
#include <io.h>
using std::string;
using std::vector;
namespace MyUtil{
	//获取某目录下所有指定扩展名文件
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
