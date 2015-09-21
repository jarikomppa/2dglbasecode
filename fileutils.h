/*  Part of Sol's 2d gl basecode
Jari Komppa http://iki.fi/sol/

This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

For more information, please refer to <http://unlicense.org/>*
*/
#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <vector>
using namespace std;

class File
{
    FILE * f;
public:
    File();
    File(FILE *aFileHandle);
    File(const char *aFilename, const char *aFileOpenTypes);
    virtual ~File();
    virtual unsigned int read_uint8();
    virtual unsigned int read_uint16();
    virtual unsigned int read_uint32();
    virtual void read_uint8(unsigned char * aDst, int aCount);
    virtual void write_uint8(unsigned char aData);
    virtual void write_uint16(unsigned short aData);
    virtual void write_uint32(unsigned int aData);
    virtual void write_uint8(const unsigned char * aSrc, int aCount);
    virtual int tell();
    virtual void seek(int aPos);
};

class MemoryFile : public File
{
public:
    vector<char> mData;
    int mDataIdx;
    MemoryFile();
    ~MemoryFile();
    virtual unsigned int read_uint8();
    virtual unsigned int read_uint16();
    virtual unsigned int read_uint32();
    virtual void write_uint8(unsigned char aData);
    virtual void write_uint16(unsigned short aData);
    virtual void write_uint32(unsigned int aData);
    virtual int tell();
    virtual void seek(int aPos);
};
#endif
