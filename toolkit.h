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
#ifndef TOOLKIT_H
#define TOOLKIT_H

////
//// Configuration
//// 

// For windowed mode
#define DESIRED_WINDOW_WIDTH (1920/2)
// For windowed mode
#define DESIRED_WINDOW_HEIGHT (1080/2)
// Desired aspect ratio. 640x480 = 4/3. Comment out to use whatever the desktop is set.
#define DESIRED_ASPECT ((float)(DESIRED_WINDOW_WIDTH)/(float)(DESIRED_WINDOW_HEIGHT))

//#define FULLSCREEN_BY_DEFAULT

//#define RESIZABLE_WINDOW

////
//// /Configuration
//// 

#ifdef _MSC_VER
#define WINDOWS_VERSION
#endif

#ifdef __APPLE__
#define OSX_VERSION
#endif

#if !defined(WINDOWS_VERSION) && !defined(OSX_VERSION)
#define LINUX_VERSION
#endif


#ifdef WINDOWS_VERSION
#include <windows.h> // needed to get GL stuff to work
#include <SDL.h>
#include "stb/stb_image.h"
#include "glad/include/glad/glad.h"
#include <GL/gl.h>
#include <GL/glu.h>
#define stricmp _stricmp
#define strdup _strdup
#endif


#ifdef OSX_VERSION
#include <SDL/SDL.h>
#include "stb/stb_image.h"
#include "glad/include/glad/glad.h"
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#define stricmp(a,b) strcasecmp((a),(b))
#endif


#ifdef LINUX_VERSION
#include <SDL/SDL.h>
#include "stb/stb_image.h"
#include "glad/include/glad/glad.h"
#include <GL/gl.h>
#include <GL/glu.h>
#define stricmp(a,b) strcasecmp((a),(b))
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "angelcodefont.h"
#include "fileutils.h"

#ifdef _MSC_VER
#pragma warning(disable:4244) 
#pragma warning(disable:4305) 
#endif

// If you're going to render widgets to the same
// UI from different source files, you can avoid
// ID collisions by defining IMGUI_SRC_ID before
// this define block:
#ifdef IMGUI_SRC_ID
#define GEN_ID ((IMGUI_SRC_ID) + (__LINE__))
#else
#define GEN_ID (__LINE__)
#endif


struct UIState
{
	int mousex;
	int mousey;
	int mousedown;

    int mousedownx;
    int mousedowny;
    int scroll;

	int hotitem;
	int activeitem;

    int lasthotitem;
    int lasthottick;

	int kbditem;
	int keyentered;
	int keymod;
	int keychar;
	
	int lastwidget;
};

extern UIState gUIState;

enum keystates
{
    KEY_UP    = 1,
    KEY_DOWN  = 2,
    KEY_LEFT  = 4,
    KEY_RIGHT = 8,
    KEY_FIRE  = 16
};

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

// Based on implementation by Chris Lomont, www.lomont.org, public domain.
// From game programming gems 7.
class WELL512
{
public:
	unsigned long mState[16];
	unsigned int mIndex;

	WELL512();
	void srand(int aSeed);
	unsigned long rand();
};

extern WELL512 gVisualRand, gPhysicsRand;
extern int gScreenWidth, gScreenHeight;
extern SDL_Window *gSDLWindow;

void initvideo(int argc);
void set2d();
void render_perfcounters(int tex_font);

GLuint load_texture(char * aFilename, int clamp = 1);
void reload_textures();
SDL_Cursor *load_cursor(const char *aFilename, int hotx, int hoty);
void setcolor(int color);
void drawrect(float x, float y, float w, float h, int color);
void drawcircle(float aX, float aY, float aRx, float aRy, int color);
void drawtexturedrect(int tex, float x, float y, float w, float h, int color);
void quickfont_drawchar(int ch, float x, float y, float w, float h);
void quickfont_drawstring(int tex, char * string, float x, float y, int color = 0xffffffff, float size = 8.0);
void drawsprite(int tex, float x, float y, float w, float h, float alpha = 0.95f);

int rect_rect_collide(float x0a, float y0a, float x1a, float y1a, float x0b, float y0b, float x1b, float y1b);
int rect_line_collide(float x0a, float y0a, float x1a, float y1a, float x0b, float y0b, float x1b, float y1b);

void rotate2d(float &x, float &y, float angle);

int imgui_button(int id, ACFont &font, const char *text, float x, float y, float w, float h, int base, int face, int hot, int textcolor);
int imgui_slider(int id, float x, float y, float w, float h, int bg, int thumb, int hot, int max, int &value, int screenful, int keyboardcontrolvalue);
int imgui_textfield(int id, ACFont &font, int x, int y, int w, int h, char *buffer, int maxlen, int base, int face, int hot, int textcolor);
void imgui_prepare();
void imgui_finish();

#endif
