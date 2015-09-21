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
#include <stdio.h>
#include <stdlib.h>
#include "toolkit.h"
#include "fileutils.h"

File::File() 
{ 
    f = NULL; 
}

File::File(FILE *aFileHandle) 
{ 
    f = aFileHandle;
}

File::File(const char *aFilename, const char *aFileOpenTypes)
{
    f = fopen(aFilename, aFileOpenTypes);
}

File::~File()
{
    if (f) fclose(f);
}

unsigned int File::read_uint8()
{
    unsigned char i = 0;
    fread(&i, 1, 1, f);
    return i;
}

unsigned int File::read_uint16()
{
    unsigned short i = 0;
    fread(&i, 2, 1, f);
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
    return i;
#else
    return (signed short)SDL_Swap16(i);
#endif
}

unsigned int File::read_uint32()
{
    unsigned int i = 0;
    fread(&i, 4, 1, f);
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
    return i;
#else
    return SDL_Swap32(i);
#endif
}

void File::read_uint8(unsigned char * dst, int count)
{
    while (count)
    {
        *dst = read_uint8();
        dst++;
        count--;
    }
}

void File::write_uint8(unsigned char data)
{
    fwrite(&data, 1, 1, f);
}

void File::write_uint16(unsigned short data)
{
#if SDL_BYTEORDER != SDL_LIL_ENDIAN
    data = SDL_Swap16(data);
#endif
    fwrite(&data, 2, 1, f);
}

void File::write_uint32(unsigned int data)
{
#if SDL_BYTEORDER != SDL_LIL_ENDIAN
    data = SDL_Swap32(data);
#endif
    fwrite(&data, 4, 1, f);
}

void File::write_uint8(const unsigned char * src, int count)
{
    while (count)
    {
        write_uint8(*src);
        src++;
        count--;
    }
}

int File::tell()
{
    return ftell(f);
}

void File::seek(int pos)
{
    fseek(f, pos, SEEK_SET);
}



MemoryFile::MemoryFile() 
{ 
    mDataIdx = 0;
}

MemoryFile::~MemoryFile()
{
}

unsigned int MemoryFile::read_uint8()
{
    unsigned char i = mData[mDataIdx];
    mDataIdx++;
    return i;
}

unsigned int MemoryFile::read_uint16()
{
    unsigned short i = 0;
    i |= (unsigned char)read_uint8(); i <<= 8;
    i |= (unsigned char)read_uint8();
    return i;
}

unsigned int MemoryFile::read_uint32()
{
    unsigned int i = 0;
    i |= (unsigned char)read_uint8(); i <<= 8;
    i |= (unsigned char)read_uint8(); i <<= 8;
    i |= (unsigned char)read_uint8(); i <<= 8;
    i |= (unsigned char)read_uint8();
    return i;
}

void MemoryFile::write_uint8(unsigned char data)
{
    mData.push_back(data);
    mDataIdx++;
}

void MemoryFile::write_uint16(unsigned short data)
{
    unsigned short d = (unsigned short)data;
    mData.push_back((d >> 8) & 0xff);
    mData.push_back((d >> 0) & 0xff);
    mDataIdx += 2;
}

void MemoryFile::write_uint32(unsigned int data)
{
    unsigned int d = (unsigned int)data;
    mData.push_back((d >> 24) & 0xff);
    mData.push_back((d >> 16) & 0xff);
    mData.push_back((d >> 8) & 0xff);
    mData.push_back((d >> 0) & 0xff);
    mDataIdx += 4;
}

int MemoryFile::tell()
{
    return mDataIdx;
}

void MemoryFile::seek(int pos)
{
    mDataIdx = pos;
}
