#include "stdafx.h"
#include "CLock.h"

namespace OwLibSV
{
	CLock::CLock()
	{
		::InitializeCriticalSection(&m_cs);
	}

	CLock::~CLock()
	{
		::DeleteCriticalSection(&m_cs);
	}

	void CLock::Lock()
	{
		::EnterCriticalSection(&m_cs);
	}

	void CLock::UnLock()
	{
		::LeaveCriticalSection(&m_cs);
	}
}
