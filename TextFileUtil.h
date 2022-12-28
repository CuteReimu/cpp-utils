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
		//���ı��ļ�
		void readLines(vector<string> &str);
		//д�ı��ļ�
		void writeLines(vector<string> &strList);
		//��ĳ�ı��ļ������ݸ��Ƶ�������
		void readToClipboard();
		//��ȡ�ı��ļ��ı����ʽ
		string getCodeString();
	};
}
