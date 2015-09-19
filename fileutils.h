/*  Part of Sol's 2d gl basecode
*
*  Copyright (C) 2015 Jari Komppa
*
*  This software is provided 'as-is', without any express or implied
*  warranty.  In no event will the authors be held liable for any damages
*  arising from the use of this software.
*
*  Permission is granted to anyone to use this software for any purpose,
*  including commercial applications, and to alter it and redistribute it
*  freely, subject to the following restrictions:
*
*  1. The origin of this software must not be misrepresented; you must not
*     claim that you wrote the original software. If you use this software
*     in a product, an acknowledgment in the product documentation would be
*     appreciated but is not required.
*  2. Altered source versions must be plainly marked as such, and must not be
*     misrepresented as being the original software.
*  3. This notice may not be removed or altered from any source distribution.
*
* Jari Komppa http://iki.fi/sol/
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
