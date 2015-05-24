#pragma once

#include "include/Leap.h"
#include "DrawMode.h"
#include "PadArtist.h"
//#include <windows.h>
#include <map>

/* Thanks to Leap Motion for providing a sample for a Listener */
class PadListener: public Leap::Listener
{
	typedef Leap::Controller Controller;
	PadArtist artist;

	int lastX = 0, lastY = 0;
	int lastX2 = 0, lastY2 = 0;
	DrawMode mode = NONE;
	DrawMode mode2 = NONE;
	cimg_library::CImgDisplay& display;

	unsigned timer = 0;
	unsigned timer2 = 0;

	bool camera = false;

public:
	PadListener(cimg_library::CImg<unsigned char>& image, cimg_library::CImgDisplay& display) :
		artist(PadArtist(image, BLACK)),
		display(display)
	{
	}

	virtual void onInit(const Controller&);
	virtual void onConnect(const Controller&);
	virtual void onDisconnect(const Controller&);
	virtual void onExit(const Controller&);
	virtual void onFrame(const Controller&);
	virtual void onFocusGained(const Controller&);
	virtual void onFocusLost(const Controller&);
	virtual void onDeviceChange(const Controller&);
	virtual void onServiceConnect(const Controller&);
	virtual void onServiceDisconnect(const Controller&);
};