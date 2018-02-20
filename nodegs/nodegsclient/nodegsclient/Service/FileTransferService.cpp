#include "..\\stdafx.h"
#include "..\\CStr.h"
#include "Binary.h"
#include "FileTransferService.h"
#include "HttpPostService.h"
#include <objbase.h>
#include "..\\CFile.h"

namespace OwLib
{

	FileTransferService::FileTransferService()
	{
		m_maxID = 0;
	}

	FileTransferService::~FileTransferService()
	{
		m_listeners.clear();
		m_runningWork.clear();
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////

	DWORD WINAPI MultithreadUploadWork(LPVOID param)
	{
		LPVOID *parameters = (LPVOID*)param;
		FileTransferService *service = (FileTransferService*)parameters[0];
		service->OnUploadWork((int)parameters[1], *(String*)parameters[2]);
		delete []parameters;
		parameters = 0;
		return 0;
	}

	void FileTransferService::OnUploadWork(int id, const String& url)
	{
        String filePath = m_runningWork[id];
        FileTransferListener *listener = m_listeners[id];
		string surl = "", sFilePath = "";
		CStr::wstringTostring(surl, url);
		CStr::wstringTostring(sFilePath, filePath);
		CurlHttp::GetInstance()->UploadBitFile(surl, sFilePath, listener);
	}

	int FileTransferService::Upload(const String& url, const String& filePath, FileTransferListener *listener)
	{
		int id = m_maxID++;
        m_runningWork[id] = filePath;
        m_listeners[id] = listener;
		void **parameters = new void*[3];
		parameters[0] = (void*)this;
		parameters[1] = (void*)id;
		parameters[2] = (void*)&url;
		CreateThread(0, 0, MultithreadUploadWork, (LPVOID)parameters, 0, 0);
		return 0;
	}
}