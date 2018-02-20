#include "stdafx.h"
#include "CFile.h"

namespace OwLibCT
{
	bool CFileA::Append(const char *file, const char *content)
	{
		fstream fs(file, ios::app);
		if(fs)
		{
			fs << content;
			fs.close();
			return true;
		}
		return false;
	}
    
	void CFileA::CreateDirectory(const char *dir)
	{
        mkdir(dir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	}
    
	bool CFileA::IsDirectoryExist(const char *dir)
	{
		if( (access(dir, 0 )) != -1 )
		{
			return true;
		}
		return false;
	}
    
	bool CFileA::IsFileExist(const char *file)
	{
		fstream fs;
		fs.open(file, ios::in);
		if(fs)
		{
			fs.close();
			return true;
		}
		else
		{
			return false;
		}
	}
    
	bool CFileA::GetDirectories(const char *dir, vector<string> *dirs)
	{
        return false;
	}
    
	int CFileA::GetFileLength(const char *file)
	{
		FILE* fp = 0;
		int fileLen = 0;
		fp = fopen(file, "rb");
		if (!fp)
		{
            return 0;
		}
		fseek(fp, 0, SEEK_END);
		fileLen = ftell(fp);
		fclose(fp);
		return fileLen;
	}
    
	bool CFileA::GetFiles(const char *dir, vector<string> *files)
	{
        return false;
	}
    
	int CFileA::GetFileState(const char *file, struct stat *buf)
	{
        return 0;
	}
    
	bool CFileA::Read(const char *file, string *content)
	{
        int fileLength = GetFileLength(file);
		char *str = new char[fileLength];
        memset(str, '\0', fileLength);
        ifstream fs(file, ios::in);
		if(fs)
		{
			while(!fs.eof())
			{
				fs.read(str, GetFileLength(file));
			}
			fs.close();
			return true;
		}
        *content = str;
        delete[] str;
        str = 0;
		return false;
	}
    
	void CFileA::RemoveFile(const char *file)
	{
        
	}
    
	bool CFileA::Write(const char *file, const char *content)
	{
		fstream fs(file, ios::out);
		if(fs)
		{
			fs << content;
			fs.close();
			return true;
		}
		return false;
	}
}
