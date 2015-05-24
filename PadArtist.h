#pragma once

#include "include/CImg.h"
#include "DrawMode.h"
#include "constants.h"
#include "include/Leap.h"

struct Color {
	Color(unsigned char r, unsigned char g, unsigned char b)
		:r(r), g(g), b(b)
	{
	}

	unsigned char r, g, b;
};

class PadArtist {
	typedef cimg_library::CImg<unsigned char> Image;

	Image& image; // the canvas
	int min_x = 0,
		min_y = 0,
		max_x = CANVAS_MAX_X,
		max_y = CANVAS_MAX_Y;
	int pos_x = max_x/2,
		pos_y = max_y/2 + 50;
	Color penColor; // the color of the pen in RGB

public:
	PadArtist(Image& canvas, Color color): penColor(color), image(canvas)
	{
	}

	void setPenColor(Color c) {
		penColor = c;
	}

	void drawImage(Leap::Image image);

	void drawLineBetween(int x1, int y1, int x2, int y2);

	void eraseAtPoint();

	void display(cimg_library::CImgDisplay&);
};