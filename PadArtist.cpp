// PadArtist.cpp
#include "PadArtist.h"
#include "constants.h"
#include <iostream>
#include "include/Leap.h"

void PadArtist::drawLineBetween(int x1, int y1, int x2, int y2)
{
	const unsigned char color[] = {penColor.r, penColor.g, penColor.b};
	image.draw_line(
		pos_x + x1,
		pos_y + (WINDOW_HEIGHT - y1),
		pos_x + x2,
		pos_y + (WINDOW_HEIGHT + y2), // if it works, it's not dumb
		color,
		2, // layer 2 so it draws on top of webcam images
		~0U, false
	);
}

cimg_library::CImg<unsigned char> convertImage(Leap::Image leap)
{
	auto result = cimg_library::CImg<unsigned char>(leap.data(), leap.width(), leap.height(), 1);
	//result.display();
	auto resizedResult = result.resize(result.width() * 2, result.height() * 2, 1, 3, 1);
	return result;
}

void PadArtist::drawImage(Leap::Image pict)
{
	auto converted = convertImage(pict);
	image.draw_image(
		min_x+(WINDOW_WIDTH/2)-(converted.width()/2),
		min_y+(WINDOW_HEIGHT/2)-(converted.height()/2),
		0, 0,
		converted
	);
}

void PadArtist::eraseAtPoint()
{
	const unsigned char color[] = CANVAS_COLOR;
	image.draw_rectangle(min_x, min_y, max_x, max_y, color);
}

void PadArtist::display(cimg_library::CImgDisplay& target)
{
	//auto cropped = image.crop(pos_x, pos_y, pos_x + WINDOW_WIDTH, pos_y + WINDOW_HEIGHT);
	//auto cropped = image.crop(pos_x, pos_y, WINDOW_WIDTH, WINDOW_HEIGHT);
	//target.render(cropped);
	target.display(image);
}
