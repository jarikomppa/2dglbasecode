/*  Sol's 2d gl basecode
 *  version 3.0
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
 *
 *************************************
 * 
 */

#include "toolkit.h"

#define TITLE "Program Title 1.0"

// How big a tick difference is considered 'time warp', i.e. skip the time
// (to avoid physics blowing up)
#define TICK_TIMEWARP 1000
// Physics ticks per second
#define PHYSICS_FPS 100

#define USE_PERFCOUNTERS

int tex_logo, tex_font;
int gKeyState = 0;
int gLastTick = 0;

ACFont fn;

void handle_key(int keysym, int down)
{
    switch(keysym)
    {
    case SDLK_ESCAPE:
        if (!down)
        {
            exit(0);
        }
        break;
    case SDLK_UP:
    case SDLK_w:
        if (down) gKeyState |= KEY_UP; else gKeyState &= ~KEY_UP;
        break;
    case SDLK_DOWN:
    case SDLK_s:
        if (down) gKeyState |= KEY_DOWN; else gKeyState &= ~KEY_DOWN;
        break;
    case SDLK_LEFT:
    case SDLK_a:
        if (down) gKeyState |= KEY_LEFT; else gKeyState &= ~KEY_LEFT;
        break;
    case SDLK_RIGHT:
    case SDLK_d:
        if (down) gKeyState |= KEY_RIGHT; else gKeyState &= ~KEY_RIGHT;
        break;
    case SDLK_RCTRL:
    case SDLK_LCTRL:
        if (down) gKeyState |= KEY_FIRE;
        break;
    }
}

void process_events()
{
    SDL_Event event;

    while (SDL_PollEvent(&event)) 
    {
        switch (event.type) 
        {
        case SDL_KEYDOWN:
            handle_key(event.key.keysym.sym, 1);
			// If a key is pressed, report it to the widgets
			gUIState.keyentered = event.key.keysym.sym;
			gUIState.keymod = event.key.keysym.mod;
			// if key is ASCII, accept it as character input
			if ((event.key.keysym.unicode & 0xFF80) == 0)
				gUIState.keychar = event.key.keysym.unicode & 0x7f;				
            break;
        case SDL_KEYUP:
            handle_key(event.key.keysym.sym, 0);
            break;
        case SDL_MOUSEMOTION:
			// update mouse position
			gUIState.mousex = (event.motion.x * (float)DESIRED_WINDOW_WIDTH) / (float)gScreenWidth;
			gUIState.mousey = (event.motion.y * (float)DESIRED_WINDOW_HEIGHT) / (float)gScreenHeight;
            break;
		case SDL_MOUSEBUTTONDOWN:
			// update button down state if left-clicking
			if (event.button.button == 1)
            {
				gUIState.mousedown = 1;
			    gUIState.mousedownx = (event.motion.x * (float)DESIRED_WINDOW_WIDTH) / (float)gScreenWidth;
			    gUIState.mousedowny = (event.motion.y * (float)DESIRED_WINDOW_HEIGHT) / (float)gScreenHeight;
            }
            if (event.button.button == 4)
            {
                gUIState.scroll = +1;
            }
            if (event.button.button == 5)
            {
                gUIState.scroll = -1;
            }
			break;
		case SDL_MOUSEBUTTONUP:
			// update button down state if left-clicking
			if (event.button.button == 1)
				gUIState.mousedown = 0;
			break;
        case SDL_QUIT:
            SDL_Quit();
            exit(0);
            break;
        case SDL_VIDEORESIZE:
            gScreenWidth = event.resize.w;
            gScreenHeight = event.resize.h;
            initvideo(0);
            break;

        }
    }
}



void physics_tick(int tick)
{
}


static void draw_screen_ingame()
{
    int tick = SDL_GetTicks();

    int i;    

    if (tick - gLastTick > TICK_TIMEWARP) 
        gLastTick = tick;

    if (gLastTick >= tick)
    {
        SDL_Delay(1);
        return;
    }

    while (gLastTick < tick)
    {
        physics_tick(gLastTick);

        gLastTick += 1000 / PHYSICS_FPS;
    }

    ////////////////////////////////////
    // Rendering
    ////////////////////////////////////

    glClearColor(0.1,0.1,0.2,1.0);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    ////////////////////////////////////
    // testcode
    ////////////////////////////////////

    drawsprite(tex_logo,64,64,128,128);

    ////////////////////////////////////
	// Let's do some 3d..
    ////////////////////////////////////

	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	gluPerspective(60,1,10,1000);

	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	gluLookAt(0,0,-100,0,0,0,0,1,0);

	glRotatef(tick * 0.1f, 5, 0, 0);

	glColor4f(1,1,1,1);

	glBegin(GL_LINES);
	for (i = 0; i < 500; i++)
	{
		glColor4f(1,1,1,i/500.0f);
		glVertex3f(sin((i + (tick / 50)*2)*0.02394857) * 20, sin((i + (tick / 50)*2)*0.0423987) * 20, sin((i + (tick / 50)*2)*0.06349857) * 20);
	}
	glEnd();


    ////////////////////////////////////
	// Back to 2d
    ////////////////////////////////////

	set2d();

    char temp[200];
    sprintf(temp, "%d %d %d %d", gScreenWidth, gScreenHeight, gUIState.mousex, gUIState.mousey);
    quickfont_drawstring(tex_font, temp, 50,400,0xffffff,1,1);

    imgui_prepare();

    static char textfield_text[200];

    if (imgui_button(GEN_ID,fn,"Hello1",50,150,64,40,0xff3f3f7f,0xff7f7f9f,0xff9f9fff,0xff000000))
    {
        char * hello = "Hello world!\0";
        memcpy(textfield_text,hello,strlen(hello)+1);
    }
    if (imgui_button(GEN_ID,fn,"Hello2",50,200,64,40,0xff3f3f7f,0xff7f7f9f,0xff9f9fff,0xff000000))
    {
        char * hello = "Hello Starsystem!\0";
        memcpy(textfield_text,hello,strlen(hello)+1);
    }
    if (imgui_button(GEN_ID,fn,"Hello3",50,250,64,40,0xff3f3f7f,0xff7f7f9f,0xff9f9fff,0xff000000))
    {
        char * hello = "Hello Galaxy!\0";
        memcpy(textfield_text,hello,strlen(hello)+1);
    }
    if (imgui_button(GEN_ID,fn,"Hello4",50,300,64,40,0xff3f3f7f,0xff7f7f9f,0xff9f9fff,0xff000000))
    {
        char * hello = "Hello Universe!\0";
        memcpy(textfield_text,hello,strlen(hello)+1);
    }

    static int slidervalue = 25;
	slidervalue *= 10;
    imgui_slider(GEN_ID,860,50,24,400,0xff3f3f7f,0xff7f7f9f,0xff9f9fff, 120*10,slidervalue, 10, 1);
	slidervalue /= 10;
    
    imgui_textfield(GEN_ID,fn,200,450,600,44,textfield_text,100,0xff3f3f7f,0xff7f7f9f,0xff9f9fff,0xff000000);

    imgui_finish();

    // Draw "mouse cursor"
    drawrect(gUIState.mousex,gUIState.mousey,16,16,0xff00ff00);
    drawrect(gUIState.mousex+16,gUIState.mousey+16,8,8,0xff007f00);
    drawrect(gUIState.mousex+12,gUIState.mousey+12,8,8,0xff007f00);
    drawrect(gUIState.mousex+8,gUIState.mousey+8,8,8,0xff007f00);

	ACFontStringMetrics m;

	char textfield_dup[200];
	memcpy(textfield_dup, textfield_text, 200);
	fn.wordwrap(textfield_dup, 400, slidervalue*1.1 + 10);

	fn.stringmetrics(textfield_dup, m, slidervalue * 1.1 + 10);
	fn.drawstring(textfield_dup, (DESIRED_WINDOW_WIDTH - m.mWidth) / 2, (DESIRED_WINDOW_HEIGHT - m.mHeight) / 2, 0x7f000000, slidervalue*1.1 + 10);
	fn.stringmetrics(textfield_dup, m, slidervalue + 10);
	fn.drawstring(textfield_dup, (DESIRED_WINDOW_WIDTH - m.mWidth) / 2, (DESIRED_WINDOW_HEIGHT - m.mHeight) / 2, 0x7fffffff, slidervalue + 10);
    

#ifdef USE_PERFCOUNTERS

	render_perfcounters(tex_font);

#endif
    SDL_Delay(10);
	SDL_GL_SwapBuffers();
}



void draw_screen()
{
    draw_screen_ingame();
}


int main (int argc, char** args)
{
    gVisualRand.srand(0xc0cac01a);

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE) < 0) 
    {
        fprintf(stderr, "Video initialization failed: %s\n", SDL_GetError());
        SDL_Quit();
        exit(0);
    }

	initvideo(argc);

    tex_logo = load_texture("spherebase.png");
    tex_font = load_texture("fona.png");

	// For imgui - Enable keyboard repeat to make sliders more tolerable
	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
	// For imgui - Enable keyboard UNICODE processing for the text field.
	SDL_EnableUNICODE(1);

    // load a font..
    fn.load("oswald-regular-120.fnt");
    
    // set window title
    SDL_WM_SetCaption(TITLE " - http://iki.fi/sol/", NULL);

    // hide cursor
    SDL_ShowCursor(0);

//	SDL_SetCursor(load_cursor("cursor_scissors.png",0,0));
    
    while (1) 
    {
        process_events();
        draw_screen();
    }

    return 0;
}
