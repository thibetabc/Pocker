//
//  Buffer.h
//  MyLib
//
//  Created by MirrorMiko on 7/30/14.
//  Copyright (c) 2014 MirrorMiko. All rights reserved.
//

#ifndef __MyLib__Buffer__
#define __MyLib__Buffer__

#include <memory>
#include <iostream>

#define DEFAULT_BUFFER_GRANULARITY	(64)

class Buffer //: public Object
{
public:
    virtual std::string ToString();
    
public:
    Buffer(size_t aGranularity = DEFAULT_BUFFER_GRANULARITY);    // 指定分配粒度构造一个Buffer
    Buffer(void* aBuffer,size_t aBufferSize,size_t aGranularity = DEFAULT_BUFFER_GRANULARITY);
    Buffer(const Buffer& buffer);                           // 复制构造函数
    Buffer& 	operator=(const Buffer& aBuffer);             // 赋值操作符
    ~Buffer();
public:
    size_t      Insert(size_t aPos,const void* aBuf,size_t aSize);
    size_t      Append(const Buffer& aBuffer);
    size_t      Append(const void* pBuf,size_t aSize);
    bool       	Remove(size_t aPos,size_t aSize);
    bool       	RemoveHead(size_t aSize);
    bool       	RemoveTail(size_t aSize);
    
    bool	   	ReadUInt8(uint8_t& val);
    bool		ReadUInt16(uint16_t& val);
    bool		ReadUInt32(uint32_t& val);
    bool		ReadUInt64(uint64_t& val);
    bool		ReadFloat(float& val);
    bool		ReadDouble(double& val);
    bool		ReadString(std::string& val);
    bool	   	ReadBytes(void* buf, size_t len);
    bool	   	WriteUInt8(uint8_t val);
    bool        WriteUInt16(uint16_t val);
    bool        WriteUInt32(uint32_t val);
    bool        WriteUInt64(uint64_t val);
    bool		WriteFloat(float val);
    bool		WriteDouble(double val);
    bool		WriteString(std::string& val);
    bool	   	WriteBytes(const void* buf, size_t len);
    bool	   	FillBytes(size_t len, unsigned char val);

    void       	ReleaseBuffer();
    void       	Clear();
    
    size_t      Length() const { return m_UsedSize; }
    void        SetLength(size_t length);
    size_t      GetPos() { return m_Pos; }
    void        SetPos(size_t pos) { m_Pos = pos; }
    size_t      MaxLength() const { return m_BufSize; }
    bool       	Realloc(size_t aNewSize);
    void*      	Data() const { return m_Buf; }
    void*      	DataInPos() const { return (char*)m_Buf + m_Pos; }
    void        SetBuffer(void* buf, size_t size);
    void*       ReplaceBuffer(void* buf, size_t size);
private:
    void*    	m_Buf;
    size_t      m_Pos;

    size_t      m_BufSize;
    size_t      m_UsedSize;
    size_t      m_Granularity;
};

typedef std::shared_ptr<Buffer> BufferPointer;

#endif /* defined(__MyLib__Buffer__) */
