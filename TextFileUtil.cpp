#include "stdafx.h"
#include "TextFileUtil.h"
using namespace std;
namespace MyUtil{

	TextFile::TextFile(const string &fileName):File(fileName){
	}

	void TextFile::readLines(vector<string> &str){
		ifstream ifs(fileName, ios::_Nocreate);
		if(ifs.fail()){
			throw ios::failure("打开文件失败");
		}
		char c;
		string tmp;
		if(!ifs) return;
		while((c=ifs.get())!=EOF){
			if('\n'==c){
				str.push_back(tmp);
				tmp = "";
			} else {
				tmp+=c;
			}
		}
		str.push_back(tmp);
		ifs.close();
	}

	void TextFile::writeLines(vector<string> &str){
		ofstream ofs(fileName);
		if(ofs.fail()){
			throw ios::failure("打开文件失败");
		}
		for(string s:str){
			ofs<<s<<endl;
		}
		ofs.close();
	}

	void TextFile::readToClipboard(){
		ifstream ifs(fileName, ios::_Nocreate);
		if(ifs.fail()){
			throw ios::failure("打开文件失败");
		}
		char c[81];
		string tmp;
		while(ifs.get(c,80,'\0')!=NULL){
			tmp += c;
		}
		setSysClipboardText(tmp);
		ifs.close();
	}

	string TextFile::getCodeString(){
		ifstream ifs(fileName, ios::_Nocreate);
		if(ifs.fail()){
			throw ios::failure("打开文件失败");
		}
		string code;
		int p = (ifs.get() << 8) + ifs.get();
		switch (p) {
		case 0xefbb:
			code = "utf-8";
			break;
		case 0xfffe:
			code = "unicode";
			break;
		case 0xfeff:
			code = "utf-16be";
			break;
		default:
			code = "gbk";
		}
		ifs.close();
		return code;
	}
}