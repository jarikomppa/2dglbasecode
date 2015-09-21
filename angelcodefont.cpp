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

void ACFontInfoBlock::load(File * f)
{
    int blocksize = f->read_uint32();
    mFontSize = f->read_uint16();
    int db = f->read_uint8();
    mSmooth  = !!(db & (1 << 7));
    mUnicode = !!(db & (1 << 6));
    mItalic  = !!(db & (1 << 5));
    mBold    = !!(db & (1 << 4));
    mCharSet = f->read_uint8();
    mStretchH = f->read_uint16();
    mAA = f->read_uint8();
    mPaddingUp = f->read_uint8();
    mPaddingRight = f->read_uint8();
    mPaddingDown = f->read_uint8();
    mPaddingLeft = f->read_uint8();
    mSpacingHoriz = f->read_uint8();
    mSpacingVert = f->read_uint8();
    mOutline = f->read_uint8();
    int fontnamelen = blocksize - 14;
    mFontName = new char[fontnamelen];
    f->read_uint8((unsigned char*)mFontName, fontnamelen);    
}

ACFontInfoBlock::~ACFontInfoBlock()
{
    delete[] mFontName;
}

ACFontInfoBlock::ACFontInfoBlock()
{
	mFontSize = 0;
	mBold = 0;
	mItalic = 0;
	mUnicode = 0;
	mSmooth = 0;
	mCharSet = 0;
	mStretchH = 0;
	mAA = 0;
	mPaddingUp = 0;
	mPaddingLeft = 0;
	mPaddingRight = 0;
	mPaddingDown = 0;
	mSpacingHoriz = 0;
	mSpacingVert = 0;
	mOutline = 0;
    
	mFontName = 0;
}    

void ACFontCommonBlock::load(File * f)
{
    int blockSize = f->read_uint32();
    mLineHeight = f->read_uint16();
    mBase = f->read_uint16();
    mScaleW = f->read_uint16();
    mScaleH = f->read_uint16();
    mPages = f->read_uint16();
    int db = f->read_uint8();
    mPacked  = !!(db & (1 << 0));
    mEncoded = !!(db & (1 << 1));
	mAlphachnl = f->read_uint8();
	mRedchnl = f->read_uint8();
	mGreenchnl = f->read_uint8();
	mBluechnl = f->read_uint8();
}

ACFontCommonBlock::ACFontCommonBlock()
{
	mLineHeight = 0;
	mBase = 0;
	mScaleW = 0;
	mScaleH = 0;
	mPages = 0;
	mPacked = 0;
	mEncoded = 0;
	mAlphachnl = 0;
	mRedchnl = 0;
	mGreenchnl = 0;
	mBluechnl = 0;
}

void ACFontPagesBlock::load(File * f, char *aPrepath)
{
    int blocksize = f->read_uint32();
    int p = f->tell();
    int l = 1;
    while (f->read_uint8()) l++;
    f->seek(p);
    mPages = blocksize / l;
    int i;
    mName = new char*[mPages];
    mGLHandle = new int[mPages];
    for (i = 0; i < mPages; i++)
    {
        mName[i] = new char[l];
        f->read_uint8((unsigned char*)mName[i], l);
        char temp[256];
        // should actually take the directory from the .fnt filename string..
        sprintf(temp,"%s%s",aPrepath, mName[i]); 
        mGLHandle[i] = load_texture(temp, 1);
    }
}

ACFontPagesBlock::~ACFontPagesBlock()
{
    int i;
    for (i = 0; i < mPages; i++)
        delete[] mName[i];
    delete[] mName;
    delete[] mGLHandle;
}

ACFontPagesBlock::ACFontPagesBlock()
{
    mPages = 0;
    mName = 0;
	mGLHandle = 0;
}

void ACFontCharBlock::load(File * f)
{
    mId = f->read_uint32();
    mX = f->read_uint16();
    mY = f->read_uint16();
    mWidth = f->read_uint16();
    mHeight = f->read_uint16();
    mXOffset = (short)f->read_uint16();
	mYOffset = (short)f->read_uint16();
	mXAdvance = (short)f->read_uint16();
    mPage = f->read_uint8();
    mChannel = f->read_uint8();
}

ACFontCharBlock::ACFontCharBlock()
{
	mId = 0;
	mX = 0;
	mY = 0;
	mWidth = 0;
	mHeight = 0;
	mXOffset = 0;
	mYOffset = 0;
	mXAdvance = 0;
	mPage = 0;
	mChannel = 0;
}

void ACFontCharsBlock::load(File * f)
{
    int blocksize = f->read_uint32();
    mCharCount = blocksize / (4 + 2 * 7 + 1 + 1);
    mChar = new ACFontCharBlock[mCharCount];
    int i;
	for (i = 0; i < mCharCount; i++)
        mChar[i].load(f);
}

ACFontCharsBlock::ACFontCharsBlock()
{
	mCharCount = 0;
    mChar = 0;
}

ACFontCharsBlock::~ACFontCharsBlock()
{
    delete[] mChar;
}


void ACFontKerningPair::load(File * f)
{
	mFirst = f->read_uint32();
    mSecond = f->read_uint32();
	mAmount = (short)f->read_uint16();
}

ACFontKerningPair::ACFontKerningPair()
{
	mFirst = 0;
	mSecond = 0;
	mAmount = 0;
}    

void ACFontKerningPairsBlock::load(File * f)
{
    int blocksize = f->read_uint32();
    mKerningPairs = blocksize / (4 + 4 + 2);
    mPair = new ACFontKerningPair[mKerningPairs];
    int i;
    for (i = 0; i < mKerningPairs; i++)
        mPair[i].load(f);
}

ACFontKerningPairsBlock::~ACFontKerningPairsBlock()
{
    delete[] mPair;
}

ACFontKerningPairsBlock::ACFontKerningPairsBlock()
{
    mKerningPairs = 0;
    mPair = 0;
}

void ACFont::load(FILE *handle, char *aPrepath)
{
    File * f = new File(handle);
    if (f)
    {
        load(f);
        delete f;
    }
}

void ACFont::load(const char *filename, char *aPrepath)
{
    File * f = new File(fopen(filename, "rb"));
    if (f)
    {
        load(f);
        delete f;
    }
}

void ACFont::load(File * f, char *aPrepath)
{
    if (f == NULL) return;
    if (f->read_uint8() != 0x42) return; // B
    if (f->read_uint8() != 0x4d) return; // M
    if (f->read_uint8() != 0x46) return; // F
    if (f->read_uint8() != 3) return;    // ver 2
    if (f->read_uint8() != 1) return;
    mInfo.load(f);
    if (f->read_uint8() != 2) return;
    mCommon.load(f);
    if (f->read_uint8() != 3) return;
    mPages.load(f, aPrepath);
    if (f->read_uint8() != 4) return;
    mChars.load(f);
    if (f->read_uint8() != 5) return; // this is ok to fail
    mKernings.load(f);         
}

ACFontCharBlock * ACFont::findcharblock(int aChar)
{
    int i;
    for (i = 0; i < mChars.mCharCount; i++)
    {
		if (mChars.mChar[i].mId == aChar)
            return &(mChars.mChar[i]);
    }
    return mChars.mChar;
}

int ACFont::findkern(int aId1, int aId2)
{
    int i;
    for (i = 0; i < mKernings.mKerningPairs; i++)
		if (mKernings.mPair[i].mFirst == aId1 &&
			mKernings.mPair[i].mSecond == aId2)
            return mKernings.mPair[i].mAmount;
    return 0;
}


void ACFont::drawstring(const char * aString, float aX, float aY, int aColor, float aDesiredHeight)
{
    float scalefactor;
	if (aDesiredHeight == 0.0f)
        scalefactor = 1.0f;
    else
		scalefactor = aDesiredHeight / mCommon.mLineHeight;
    
    int currentpage = 0;
    if (mPages.mPages == 0)
        return;
	
    float xofs, yofs;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, mPages.mGLHandle[0]);
    glColor4f(((aColor >> 16) & 0xff) / 256.0f,
              ((aColor >> 8) & 0xff) / 256.0f,
              ((aColor >> 0) & 0xff) / 256.0f,
              ((aColor >> 24) & 0xff) / 256.0f);
    xofs = aX;
    yofs = aY;
    int lastid = 0;
	glBegin(GL_TRIANGLE_STRIP);

	while (*aString)
    {
		xofs += findkern(lastid, *aString) * scalefactor;
		lastid = *aString;
		if (*aString == '\n')
        {
            xofs = aX;
            yofs += mCommon.mLineHeight * scalefactor;
        }
        else
        {
			ACFontCharBlock *curr = findcharblock(*aString);
            if (curr->mPage != currentpage)
            {
                currentpage = curr->mPage;
                glBindTexture(GL_TEXTURE_2D, mPages.mGLHandle[currentpage]);
            }

			float x0 = xofs + curr->mXOffset * scalefactor;
			float u0 = curr->mX / (float)mCommon.mScaleW;

			float y0 = yofs + curr->mYOffset * scalefactor;
			float v0 = curr->mY / (float)mCommon.mScaleH;

			float x1 = xofs + (curr->mXOffset + curr->mWidth) * scalefactor;
			float u1 = (curr->mX + curr->mWidth) / (float)mCommon.mScaleW;

			float y1 = yofs + (curr->mYOffset + curr->mHeight) * scalefactor;
			float v1 = (curr->mY + curr->mHeight) / (float)mCommon.mScaleH;
			glVertex2f(x0, y0);
			glVertex2f(x0, y0);

			glTexCoord2f(u0, v0);
			glVertex2f(x0, y0);

			glTexCoord2f(u0, v1);
			glVertex2f(x0, y1);

			glTexCoord2f(u1, v0);
			glVertex2f(x1, y0);

			glTexCoord2f(u1, v1);
			glVertex2f(x1, y1);

			glVertex2f(x1, y1);
			glVertex2f(x1, y1);
			xofs += curr->mXAdvance * scalefactor;
        }
        aString++;
    }
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void ACFont::stringmetrics(const char * aString, ACFontStringMetrics &aMetrics, float aDesiredHeight)
{
    float scalefactor;
	if (aDesiredHeight == 0.0f)
        scalefactor = 1.0f;
    else
		scalefactor = aDesiredHeight / mCommon.mLineHeight;

	aMetrics.mWidth = 0;
	aMetrics.mWidth = 0;
	aMetrics.mLastLineWidth = 0;

    if (mPages.mPages == 0)
    {
        return;
    }
    float maxx = 0;
    
    float xofs, yofs;
    xofs = 0;
    yofs = 0;
    int lastid = 0;
	while (*aString)
    {
		xofs += findkern(lastid, *aString) * scalefactor;
		lastid = *aString;
		if (*aString == '\n')
        {
            xofs = 0;
            yofs += mCommon.mLineHeight * scalefactor;
        }
        else
        {
			ACFontCharBlock *curr = findcharblock(*aString);
            xofs += curr->mXAdvance * scalefactor;
        }
        if (maxx < xofs) maxx = xofs;
		aString++;
    }
    aMetrics.mLastLineWidth = xofs;
    aMetrics.mWidth = maxx;
    aMetrics.mHeight = yofs + mCommon.mLineHeight * scalefactor;
}

void ACFont::wordwrap(char *aString, float aAreaWidth, float aDesiredHeight)
{
	char *left = aString;
	char *lastspace = 0;
	ACFontStringMetrics metrics;
	while (*aString)
	{
		if (*aString == ' ')
		{
			*aString = 0;
			stringmetrics(left, metrics, aDesiredHeight);
			*aString = ' ';
			if (metrics.mWidth > aAreaWidth)
			{
				if (lastspace)
				{
					*lastspace = '\n';
				}
				else
				{
					*aString = '\n';
				}
				lastspace = 0;
				left = aString++;
			}
			else
			{
				lastspace = aString;
			}
		}
		if (*aString == '\n')
		{
			lastspace = 0;
			left = aString++;
		}
		aString++;
	}
}