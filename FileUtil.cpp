#include "stdafx.h"
#include "FileUtil.h"
using namespace std;

namespace MyUtil{
	void getFiles(const string &path, const string &ext, vector<string>& files)
	{
		//文件句柄
		long hFile   =   0;
		//文件信息
		struct _finddata_t fileinfo;
		string pathName, exdName;

		if (0 != strcmp(ext.c_str(), ""))
		{
			exdName = "\\*." + ext;
		} else {
			exdName = "\\*";
		}
	
		if((hFile = _findfirst(pathName.assign(path).append(exdName).c_str(),&fileinfo)) !=  -1)
		{
			do
			{
				//如果是文件夹中仍有文件夹,迭代之
				//如果不是,加入列表
				if((fileinfo.attrib & _A_SUBDIR))
				{
					if(strcmp(fileinfo.name,".") != 0  &&  strcmp(fileinfo.name,"..") != 0)
						getFiles( pathName.assign(path).append("\\").append(fileinfo.name), ext, files );
				} else {
					if(strcmp(fileinfo.name,".") != 0  &&  strcmp(fileinfo.name,"..") != 0)
						files.push_back(pathName.assign(path).append("\\").append(fileinfo.name));
				}
			}while(_findnext(hFile, &fileinfo)  == 0);
			_findclose(hFile);
		}
	}

	File::File(const string &fileName)
	{
		this->fileName = fileName;
	}


	File::~File(void)
	{
	}
}
