#pragma once

#include <Windows.h>
#include <TCHAR.H>
#include "OpenGLInterface.h"

class Text2D
{
public:
	Text2D() : cursor_x(0), cursor_y(0)	{ }

	void init(int width, int height, const char* font = nullptr);
	void Shutdown();
	void draw();

	void drawText(const char* str, int x, int y);
	void print(const char* str);
	void scroll(int lines);
	void moveCursor(int x, int y);
	void clear();

private:
	GLuint      text_buffer;
	GLuint      font_texture;
	GLuint      vao;

	GLuint      text_program;
	char *      screen_buffer;
	int         buffer_width;
	int         buffer_height;
	bool        dirty;
	int         cursor_x;
	int         cursor_y;
};