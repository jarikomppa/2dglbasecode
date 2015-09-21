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
