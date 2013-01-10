#ifndef UNITTEST_MEMORYOUTSTREAM_H
#define UNITTEST_MEMORYOUTSTREAM_H

#include "../config.h"
#include "HelperMacros.h"

#ifdef UNITTEST_MEMORYOUTSTREAM_IS_STD_OSTRINGSTREAM

#include <sstream>

namespace UnitTest
{

class UNITTEST_LINKAGE MemoryOutStream : public std::ostringstream
{
public:
    MemoryOutStream() {}
    ~MemoryOutStream() {}
	void Clear();
	char const* GetText() const;

private:
    MemoryOutStream(MemoryOutStream const&);
    void operator =(MemoryOutStream const&);

    mutable std::string m_text;
};

}

#else

#include <cstddef>

namespace UnitTest
{

class UNITTEST_LINKAGE MemoryOutStream
{
public:
    explicit MemoryOutStream(int const size = 256);
    ~MemoryOutStream();

	void Clear();
    char const* GetText() const;

    MemoryOutStream& operator <<(char const* txt);
    MemoryOutStream& operator <<(int n);
    MemoryOutStream& operator <<(long n);
	MemoryOutStream& operator <<(long long n);
    MemoryOutStream& operator <<(unsigned long n);
	MemoryOutStream& operator <<(unsigned long long n);
    MemoryOutStream& operator <<(float f);
    MemoryOutStream& operator <<(double d);
    MemoryOutStream& operator <<(void const* p);
    MemoryOutStream& operator <<(unsigned int s);

    enum { GROW_CHUNK_SIZE = 32 };
    int GetCapacity() const;

private:
    void operator= (MemoryOutStream const&);
    void GrowBuffer(int capacity);

    int m_capacity;
    char* m_buffer;
};

}

#endif

#endif
