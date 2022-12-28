#pragma once
#include <fstream>
#include <vector>
#include <io.h>
#include <string>
#include "ClipboardUtil.h"
#include "FileUtil.h"
using std::string;
using std::vector;
namespace MyUtil{
	class TextFile:public File{
	public:
		TextFile(const string &fileName);
		//读文本文件
		void readLines(vector<string> &str);
		//写文本文件
		void writeLines(vector<string> &strList);
		//将某文本文件的内容复制到剪贴板
		void readToClipboard();
		//获取文本文件的编码格式
		string getCodeString();
	};
}
