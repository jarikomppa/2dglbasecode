# 2dglbasecode
Sol's 2d GL basecode
http://iki.fi/sol/

This is a bunch of little helper functions I use to make small test projects and
games and stuff. Feel free to use it if you want, or steal bits if you need them.
Licensed under unlicense (as close to public domain as you can get).

![ScreenShot](https://raw.github.com/jarikomppa/2dglbasecode/master/screenshot.jpg)

# Features

Features include..

- Set up your own coordinate system, framework scales to full screen adding borders if aspect ratio doesn't match. 
- Simple texture management that handles window resizes etc.
- Random number generator
- Simple primitives - drawrect, drawcircle, drawtexturedrect, etc.
- "quick" font using BIOS font
- Angelcode bitmap font loader & renderer
- Simple collisions - rect/rect, rect/line, etc.
- IMGUI widgets for button, slider, textfield - enough for really simple UIs

# External dependencies 

- stb_image (public domain)
- Glad opengl extension loader, generated through http://glad.dav1d.de/
- SDL 1.2
