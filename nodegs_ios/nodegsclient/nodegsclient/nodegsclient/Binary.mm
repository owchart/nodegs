#include "stdafx.h"
#include "Binary.h"
#include <sys/mman.h>

namespace OwLibCT
{
	unsigned int Binary::m_dwPageSize = 0;
    
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
	unsigned int Binary::DeAllocateBuffer(unsigned int nRequestedSize)
    {
		if(nRequestedSize == 0)
		{
			return 0;
		}
		if(m_bSustainSize)
		{
			return 0;
		}
		if(m_nSize <= m_nInitSize)
		{
			return 0;
		}
		if(nRequestedSize < m_nDataSize)
		{
			return 0;
		}
		if(nRequestedSize < m_nInitSize)
		{
			nRequestedSize = m_nInitSize;
		}
		unsigned int nNewSize = m_nSize;
		while(nNewSize >= nRequestedSize * 2)
		{
			nNewSize /= 2;
		}
		if(nNewSize == m_nSize)
		{
			return 0;
		}
		PBYTE pNewBuffer = (PBYTE)valloc(nNewSize);
        mprotect(pNewBuffer, nNewSize, PROT_READ|PROT_EXEC|PROT_WRITE);
		if(m_pBase)
		{
			if(m_nDataSize)
			{
				memcpy(pNewBuffer, m_pBase, m_nDataSize);
			}
            free(m_pBase);
		}
		m_pBase = pNewBuffer;
		m_nSize = nNewSize;
		return m_nSize;
	}
    
	unsigned int Binary::GetMemSize()
	{
		return m_nSize;
	}
    
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
	Binary::Binary()
	{
		m_bSingleRead = true;
		m_bSustainSize = false;
		m_nDataSize = 0;
		m_nInitSize = 0;
		m_nReadPos = 0;
		m_nSize = 0;
		m_pBase =  0;
		if(m_dwPageSize == 0)
		{
            int dwPageSize = 4096;
            m_dwPageSize = dwPageSize;
			while(m_dwPageSize < 8192)
			{
				m_dwPageSize += dwPageSize;
			}
        }
	}
    
	Binary::~Binary()
	{
		if(m_pBase)
		{
            free(m_pBase);
		}
	}
    
	const Binary& Binary::operator+(Binary& buff)
	{
		this->Write(buff.GetBytes(), buff.GetBytesLen());
		return *this;
	}
    
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
	void Binary::ClearBytes()
	{
		m_nDataSize = 0;
		m_nReadPos = 0;
		DeAllocateBuffer(0);
	}
    
	void Binary::Copy(Binary& buffer)
	{
        unsigned int nReSize = buffer.GetMemSize();
		if(nReSize != m_nSize)
		{
			if (m_pBase)
			{
                free(m_pBase);
			}
            m_pBase = (PBYTE)valloc(nReSize);
            mprotect(m_pBase, nReSize, PROT_READ|PROT_EXEC|PROT_WRITE);
			m_nSize = nReSize;
		}
		m_nDataSize = buffer.GetBytesLen();
		if(m_nDataSize > 0)
		{
			memcpy(m_pBase, buffer.GetBytes(), m_nDataSize);
		}
	}
    
	unsigned int Binary::Delete(unsigned int nSize)
	{
        if(nSize == 0)
		{
			return nSize;
		}
		if (nSize > m_nDataSize)
		{
			nSize = m_nDataSize;
		}
		m_nDataSize -= nSize;
		if(m_nDataSize > 0)
		{
			memmove(m_pBase, m_pBase+nSize, m_nDataSize);
		}
		DeAllocateBuffer(m_nDataSize);
		return nSize;
	}
    
	unsigned int Binary::DeleteEnd(unsigned int nSize)
	{
		if(nSize > m_nDataSize)
			nSize = m_nDataSize;
		if(nSize)
		{
			m_nDataSize -= nSize;
			DeAllocateBuffer(m_nDataSize);
		}
		return nSize;
	}
    
	void Binary::FileRead(const String& strFileName)
	{
	}
    
	void Binary::FileWrite(const String& strFileName)
	{
	}
    
	PBYTE Binary::GetBytes(unsigned int nPos)
	{
		if(!m_pBase)
		{
			return 0;
		}
		return m_pBase + nPos;
	}
    
	unsigned int Binary::GetBytesLen()
	{
		if(!m_pBase)
		{
			return 0;
		}
		if (m_bSingleRead)
		{
			return m_nDataSize - m_nReadPos;
		}
		else
		{
			return m_nDataSize;
		}
	}
    
	void Binary::Initialize(unsigned int nInitsize, bool bSustain)
	{
		m_bSustainSize = bSustain;
		ReAllocateBuffer(nInitsize);
		m_nInitSize = m_nSize;
	}
    
	unsigned int Binary::Insert(const void *pData, unsigned int nSize)
	{
        ReAllocateBuffer(nSize + m_nDataSize);
		memmove(m_pBase + nSize, m_pBase, m_nDataSize);
		memcpy(m_pBase, pData, nSize);
		m_nDataSize += nSize;
		return nSize;
	}
    
	unsigned int Binary::Insert(String& strData)
	{
		int nSize = (int)strData.size();
		return Insert((const void*) strData.c_str(), nSize);
	}
    
	unsigned int Binary::Read(void *pData, unsigned int nSize)
	{
        if (nSize)
		{
			if (m_bSingleRead)
			{
				if (nSize+m_nReadPos > m_nDataSize)
				{
					throw DATA_LACK;
					return 0;
				}
				memcpy(pData, m_pBase + m_nReadPos, nSize);
				m_nReadPos += nSize;
			}
			else
			{
				if (nSize > m_nDataSize)
				{
					throw DATA_LACK;
					return 0;
				}
				m_nDataSize -= nSize;
				memcpy(pData, m_pBase, nSize);
				if (m_nDataSize > 0)
				{
					memmove(m_pBase, m_pBase + nSize, m_nDataSize);
				}
			}
		}
		DeAllocateBuffer(m_nDataSize);
		return nSize;
	}
    
	char Binary::ReadChar()
	{
		char cValue;
		Read(&cValue, sizeof(char));
		return cValue;
	}
    
	double Binary::ReadDouble()
	{
		double dValue;
		Read(&dValue, sizeof(double));
		return dValue;
	}
    
	float Binary::ReadFloat()
	{
		float fValue;
		Read(&fValue, sizeof(float));
		return fValue;
	}
    
	int Binary::ReadInt()
	{
		int nValue;
		Read(&nValue, sizeof(int));
		return nValue;
	}
    
	_int64 Binary::ReadLong()
	{
        return 0;
	}
    
	short Binary::ReadShort()
	{
		short sValue;
		Read(&sValue, sizeof(short));
		return sValue;
	}
    
	unsigned int Binary::ReadString(String& strData)
	{
		strData.clear();
		unsigned int dwSize;
		Read(&dwSize, sizeof(unsigned int));
		if(dwSize == 0)
		{
			return 2;
		}
		if (m_bSingleRead)
		{
			if (dwSize + m_nReadPos > m_nDataSize)
			{
				throw DATA_LACK;
				return 0;
			}
			string str((const char*)m_pBase+m_nReadPos, dwSize);
            CStrA::stringTowstring(strData, str);
			m_nReadPos += dwSize;
		}
		else
		{
			if (dwSize > m_nDataSize)
			{
				throw DATA_LACK;
				return 0;
			}
			string str((const char*)m_pBase,dwSize);
            CStrA::stringTowstring(strData, str.c_str());
			Delete(dwSize);
		}
		return (dwSize+sizeof(unsigned int));
	}
    
	unsigned int Binary::ReadString(string& strData)
	{
		strData.clear();
		unsigned int dwSize;
		Read(&dwSize, sizeof(unsigned int));
		if(dwSize == 0)
		{
			return 2;
		}
		if (m_bSingleRead)
		{
			if (dwSize+m_nReadPos > m_nDataSize)
			{
				throw DATA_LACK;
				return 0;
			}
			string str((const char*)m_pBase+m_nReadPos,dwSize);
			strData = str;
			m_nReadPos += dwSize;
		}
		else
		{
			if (dwSize > m_nDataSize)
			{
				throw DATA_LACK;
				return 0;
			}
			string str((const char*)m_pBase,dwSize);
			strData = str;
			Delete(dwSize);
		}
		return (dwSize + sizeof(unsigned int));
	}
    
	unsigned int Binary::ReAllocateBuffer(unsigned int nRequestedSize)
	{
        if(nRequestedSize <= m_nSize)
		{
			return 0;
		}
		unsigned int nNewSize = m_nSize;
		if(nNewSize < m_dwPageSize)
		{
			nNewSize = m_dwPageSize;
		}
		while(nRequestedSize > nNewSize)
		{
			nNewSize *= 2;
		}
		PBYTE pNewBuffer = (PBYTE)valloc(nNewSize);
        mprotect(pNewBuffer, nNewSize, PROT_READ|PROT_EXEC|PROT_WRITE);
		if (m_pBase)
		{
			if(m_nDataSize)
			{
                memcpy(pNewBuffer, m_pBase, m_nDataSize);
			}
            free(m_pBase);
		}
		m_pBase = pNewBuffer;
		m_nSize = nNewSize;
		return m_nSize;
	}
    
	unsigned int Binary::SkipData(int nSize)
	{
        if(m_bSingleRead)
		{
			if (nSize + m_nReadPos > m_nDataSize)
			{
				throw DATA_LACK;
				return 0;
			}
			m_nReadPos += nSize;
			return nSize;
		}
		else
		{
			m_nDataSize -= nSize;
			if (m_nDataSize > 0)
			{
                memmove(m_pBase, m_pBase + nSize, m_nDataSize);
			}
			DeAllocateBuffer(m_nDataSize);
		}
		return 0;
    }
    
	unsigned int Binary::Write(const void *pData, unsigned int nSize)
	{
        if(nSize)
		{
			ReAllocateBuffer(nSize + m_nDataSize);
            memcpy(m_pBase + m_nDataSize, pData, nSize);
			m_nDataSize += nSize;
		}
		return nSize;
    }
    
	unsigned int Binary::Write(String& strData)
	{
		int nSize = (int)strData.size();
		return Write((const void*)strData.c_str(), nSize);
	}
    
	void Binary::WriteChar(char cValue)
	{
		Write(&cValue, sizeof(char));
	}
    
	void Binary::WriteDouble(double dValue)
	{
		Write(&dValue, sizeof(double));
	}
    
	void Binary::WriteFloat(float fValue)
	{
		Write(&fValue, sizeof(float));
	}
    
	void Binary::WriteInt(int nValue)
	{
		Write(&nValue, sizeof(int));
	}
    
	void Binary::WriteLong(_int64 hValue)
	{
	}
    
	void Binary::WriteShort(short sValue)
	{
		Write(&sValue, sizeof(short));
	}
    
	unsigned int Binary::WriteString(String& strData)
	{
		string str;
        CStrA::wstringTostring(str, strData);
		unsigned int dwSize = (int)str.size();
		Write(&dwSize, sizeof(unsigned int));
		if(dwSize)
		{
			Write((const void*) str.c_str(), dwSize);
		}
		return (unsigned int)(dwSize + sizeof(unsigned int));
	}
    
	unsigned int Binary::WriteString(string& strData)
	{
		unsigned int wSize = (int)strData.size();
		Write(&wSize, sizeof(unsigned int));
		if(wSize)
		{
			Write((const void*) strData.c_str(), wSize);
		}
		return (unsigned int)(wSize + sizeof(unsigned int));
	}
}
