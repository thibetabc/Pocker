//
//  Buffer.cpp
//  MyLib
//
//  Created by MirrorMiko on 7/30/14.
//  Copyright (c) 2014 MirrorMiko. All rights reserved.
//

#include <arpa/inet.h>
#include "Buffer.h"
#include <sstream>

std::string Buffer::ToString()
{
    std::ostringstream os;
    os << "{buf:0x";
    os.fill('0');
    os.width(8);
    os << std::uppercase << std::hex << m_Buf << std::dec;
    os.fill();
    os.width();
	os << ",bufSize:" << m_BufSize
    << ",usedSize:" << m_UsedSize
    << ",granularity:" << m_Granularity
    << "}" << std::endl;
    
	return os.str();
}

Buffer::Buffer(size_t aGranularity /*= DEFAULT_BUFFER_GRANULARITY*/)
{
    m_Buf = nullptr;
    if (aGranularity != 0) {
        m_Buf          = (void*)malloc(aGranularity);
        m_Granularity  = aGranularity;
        m_BufSize      = aGranularity;
    }else{
        m_Granularity  = DEFAULT_BUFFER_GRANULARITY;
        m_BufSize      = DEFAULT_BUFFER_GRANULARITY;
    }
	m_UsedSize     = 0;
    m_Pos          = 0;
}

Buffer::Buffer(void* aBuffer, size_t aBufferSize, size_t aGranularity /*= DEFAULT_BUFFER_GRANULARITY*/)
{
    m_Buf          = (void*)malloc(aGranularity);
	m_BufSize      = aGranularity;
	m_UsedSize     = 0;
	m_Granularity  = aGranularity;
    m_Pos          = 0;
    
	this->Insert(m_UsedSize, aBuffer, aBufferSize);
}

Buffer::Buffer(const Buffer& buffer)
{
    this->m_BufSize = buffer.m_BufSize;
	this->m_Buf = malloc(this->m_BufSize);
    this->m_UsedSize = buffer.m_UsedSize;
    this->m_Pos = buffer.m_Pos;
	memcpy(this->m_Buf, buffer.Data(), this->m_UsedSize);
	this->m_Granularity = buffer.m_Granularity;
}

Buffer& Buffer::operator=(const Buffer& aBuffer)
{
    if(this == &aBuffer)
    {
        return *this;
    }
    
    this->Clear();
    this->m_Granularity = aBuffer.m_Granularity;
	this->Append(aBuffer);
    
	return *this;
}

Buffer::~Buffer()
{
    if (m_Buf != nullptr)
    {
        free(m_Buf);
        m_Buf = nullptr;
    }
}

size_t Buffer::Insert(size_t aPos, const void* aBuf, size_t aSize)
{
    if(aSize <= 0)
		return m_UsedSize;
    
	size_t nSumSize = m_UsedSize + aSize;
	if(nSumSize > m_BufSize)
	{
		size_t nNewSize = ((nSumSize + m_Granularity - 1) / m_Granularity) * m_Granularity;
		if(!this->Realloc(nNewSize))
			return	SIZE_T_MAX;
	}
    
	if(aPos >= m_UsedSize)
	{
		// 插在后面
		if(aBuf != NULL)
			memcpy((char*)m_Buf + aPos, aBuf, aSize);
	}
	else
	{
		char* move_data = (char*)malloc(m_UsedSize - aPos);
		memcpy(move_data, (char*)m_Buf + aPos, m_UsedSize - aPos);
		if(aBuf != NULL)
			memcpy((char*)m_Buf + aPos,aBuf,aSize);
		memcpy((char*)m_Buf + aPos + aSize, move_data, m_UsedSize - aPos);
		free(move_data);
	}
    
	m_UsedSize += aSize;
	return	m_UsedSize;
}

size_t Buffer::Append(const Buffer& aBuffer)
{
    return this->Append(aBuffer.Data(), aBuffer.Length());
}

size_t Buffer::Append(const void* pBuf, size_t aSize)
{
    return this->Insert(m_UsedSize, pBuf, aSize);
}

bool Buffer::Remove(size_t aPos, size_t nSize)
{
    if((nSize <= 0) || (aPos >= m_UsedSize))
		return false;
    
	if(aPos + nSize > m_UsedSize)
		nSize = m_UsedSize - aPos;
    
	char* pSrc = (char*)m_Buf + aPos + nSize;
	char* pDes = (char*)m_Buf + aPos;
	for(size_t n = aPos; n < m_UsedSize - nSize; n++)
	{
		*pDes = *pSrc;
		pDes += 1;
		pSrc += 1;
	}
    
	m_UsedSize -= nSize;
    
	return true;
}

bool Buffer::RemoveHead(size_t nSize)
{
    return this->Remove(0, nSize);
}

bool Buffer::RemoveTail(size_t aSize)
{
    if( aSize > m_UsedSize )
		aSize = m_UsedSize;
    
	return this->Remove(m_UsedSize - aSize, aSize);
}

/////////////////// 序列化、反序列化使用,序列化都会在尾部写入数据,反序列化都会从头部删除数据
bool Buffer::ReadUInt8(uint8_t& val)
{
    size_t len = sizeof(val);
	if(this->Length() < len)
		return false;
    
	memcpy(&val, this->Data(), len);
    
	return this->RemoveHead(len);
}

bool Buffer::ReadUInt16(uint16_t& val)
{
    size_t len = sizeof(val);
	if(this->Length() < len)
		return false;
    
	memcpy(&val,this->Data(),len);
    val = htons(val);
	return this->RemoveHead(len);
}

bool Buffer::ReadUInt32(uint32_t& val)
{
    size_t len = sizeof(val);
	if(this->Length() < len)
		return false;
    
	memcpy(&val,this->Data(),len);
    val = htonl(val);
	return this->RemoveHead(len);
}

/*
bool Buffer::ReadUInt64(uint64_t& val)
{
    size_t len = sizeof(val);
	if(this->Length() < len)
		return false;
    
	memcpy(&val,this->Data(),len);
    val = htonll(val);
	return this->RemoveHead(len);
}
*/

bool Buffer::ReadFloat(float& val)
{
    size_t len = sizeof(val);
	if(this->Length() < len)
		return false;
    
	memcpy(&val,this->Data(),len);
    
	return this->RemoveHead(len);
}

bool Buffer::ReadDouble(double& val)
{
    size_t len = sizeof(val);
	if(this->Length() < len)
		return false;
    
	memcpy(&val,this->Data(),len);
    
	return this->RemoveHead(len);
}

bool Buffer::ReadString(std::string& val)
{
    uint16_t str_len = 0;
    
	size_t len = sizeof(str_len);
	if(this->Length() < len)
		return false;
    
	this->ReadUInt16(str_len);
    
	if(this->Length() < str_len)
		return false;
    
	char* p = new char[str_len];
	memset((void*)p, 0, str_len);
    
	memcpy(p, this->Data(), str_len);
    
	val = p;
	delete [] p;
    
	return this->RemoveHead(str_len);
}

bool Buffer::ReadBytes(void* buf, size_t len)
{
    if(this->Length() < len)
		return false;
    
	memcpy(buf, this->Data(), len);
    
	return this->RemoveHead(len);
}

bool Buffer::WriteUInt8(uint8_t val)
{
    return this->Append(&val, sizeof(val)) != SIZE_T_MAX;
}

bool Buffer::WriteUInt16(uint16_t val)
{
    uint16_t revert = HTONS(val);
    return this->Append(&revert, sizeof(val)) != SIZE_T_MAX;
}

bool Buffer::WriteUInt32(uint32_t val)
{
    uint32_t revert = HTONL(val);
    return this->Append(&revert, sizeof(val)) != SIZE_T_MAX;
}

/*
bool Buffer::WriteUInt64(uint64_t val)
{
    uint64_t revert = HTONLL(val);
    return this->Append(&revert, sizeof(revert)) != SIZE_T_MAX;
}
*/

bool Buffer::WriteFloat(float val)
{
    return this->Append(&val, sizeof(val)) != SIZE_T_MAX;
}

bool Buffer::WriteDouble(double val)
{
    return this->Append(&val, sizeof(val)) != SIZE_T_MAX;
}

bool Buffer::WriteString(std::string& val)
{
    this->WriteUInt16(val.size()+1);
	return this->WriteBytes((void*)val.c_str(), val.size() + 1);
}

bool Buffer::WriteBytes(const void* buf, size_t len)
{
    if(buf == NULL || len == 0)
		return false;
    
	return this->Append(buf,len) != SIZE_T_MAX;
}

bool Buffer::FillBytes(size_t len, unsigned char val){
    void* buf = malloc(len);
    memset(buf, 0, len);
    bool ok = this->Append(buf, len) != SIZE_T_MAX;
    free(buf);
    return ok;
}
///////////////////

void Buffer::ReleaseBuffer()
{
    size_t nNewSize = ((m_UsedSize + m_Granularity - 1) / m_Granularity) * m_Granularity;
	this->Realloc(nNewSize);
}

void Buffer::Clear()
{
    free(m_Buf);
    
	m_Buf          = (void*)malloc(m_Granularity);
	m_BufSize      = m_Granularity;
	m_UsedSize     = 0;
}

void Buffer::SetLength(size_t length)
{
    if (length > m_BufSize) {
        return ;
    }else {
        m_UsedSize = length;
    }
}

bool Buffer::Realloc(size_t nSize)
{
    if(nSize >= m_UsedSize)
	{
		size_t nNewSize = nSize;
		void* pNewBuf = NULL;
		if (m_Buf)
			pNewBuf = (void*)realloc(m_Buf, nNewSize);
		else
			pNewBuf = (void*)malloc(nNewSize);
        
		if (pNewBuf)
		{
			m_Buf = pNewBuf;
			m_BufSize = nNewSize;
            
			return true;
		}
		else
			return false;
	}
	else
	    return false;
}

void Buffer::SetBuffer(void* buf, size_t size){
    if (m_Buf){
        free(m_Buf);
    }
    m_Buf = buf;
    SetLength(0);
    SetPos(0);
    m_BufSize = size;
}

void* Buffer::ReplaceBuffer(void* buf, size_t size){
    void* ob = m_Buf;
    m_Buf = buf;
    SetLength(0);
    SetPos(0);
    m_BufSize = size;
    return ob;
}
