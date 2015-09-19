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
#ifndef ANGELCODEFONT_H
#define ANGELCODEFONT_H
// Angelcode bitmap font generator font loader, as of 1.14 beta (format version 3)

class File;

class ACFontInfoBlock
{
public:
    int mFontSize;
    int mBold;
    int mItalic;
    int mUnicode;
    int mSmooth;
    int mCharSet;
    int mStretchH;
    int mAA;
    int mPaddingUp;
    int mPaddingRight;
    int mPaddingDown;
    int mPaddingLeft;
    int mSpacingHoriz;
    int mSpacingVert;
    int mOutline;
    char * mFontName;
    void load(File * f);
    ~ACFontInfoBlock();
    ACFontInfoBlock();
};

class ACFontCommonBlock
{
public:
    int mLineHeight;
    int mBase;
    int mScaleW;
    int mScaleH;
    int mPages;
    int mPacked;
    int mEncoded;
	int mAlphachnl;
	int mRedchnl;
	int mGreenchnl;
	int mBluechnl;
    void load(File * f);
    ACFontCommonBlock();
};

class ACFontPagesBlock
{
public:
    int mPages;
    int *mGLHandle;
    char **mName;
    void load(File * f, char * aPrepath);
    ~ACFontPagesBlock();
    ACFontPagesBlock();
};


class ACFontCharBlock
{
public:
    int mId;
    int mX;
    int mY;
    int mWidth;
    int mHeight;
    int mXOffset;
    int mYOffset;
    int mXAdvance;
    int mPage;
    int mChannel;
    void load(File * f);
    ACFontCharBlock();
};

class ACFontCharsBlock
{
public:
    int mCharCount;
    ACFontCharBlock *mChar;
    void load(File * f);
    ACFontCharsBlock();
    ~ACFontCharsBlock();
};


class ACFontKerningPair
{
public:
    int mFirst;
    int mSecond;
    int mAmount;
    void load(File * f);
    ACFontKerningPair();
};

class ACFontKerningPairsBlock
{
public:
    int mKerningPairs;
    ACFontKerningPair * mPair;
    void load(File * f);
    ~ACFontKerningPairsBlock();
    ACFontKerningPairsBlock();
};

struct ACFontStringMetrics
{
	float mWidth;
	float mHeight;
	float mLastLineWidth;
};

class ACFont
{
public:
    ACFontInfoBlock mInfo;
    ACFontCommonBlock mCommon;
    ACFontPagesBlock mPages;
    ACFontCharsBlock mChars;
    ACFontKerningPairsBlock mKernings;
    void load(const char *aFilename, char *aPrepath="");
    void load(FILE * f, char *aPrepath="");
    void load(File * f, char *aPrepath="");
    void drawstring(const char * aString, float x, float y, int color=0xffffffff, float aDesiredHeight = 0.0f);
    void stringmetrics(const char * aString, ACFontStringMetrics &aMetrics, float aDesiredHeight = 0.0f);
	void wordwrap(char *aString, float aAreaWidth, float aDesiredHeight = 0.0f);
protected:
    ACFontCharBlock *findcharblock(int aChar);
    int findkern(int aId1, int aId2);
};

#endif
