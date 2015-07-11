// PadListener.cpp
#include "PadListener.h"
#include "PadArtist.h"
#include "constants.h"
#include <map>
#include <ctime>

using namespace Leap;

bool isIndexFingerStraight(const Finger& indexFinger)
{
	return indexFinger.isValid() && indexFinger.isExtended();
}

bool areFingersInPictureShape(const FingerList& fingers)
{
	auto index = fingers.fingerType(Finger::TYPE_INDEX)[0];
	auto middle = fingers.fingerType(Finger::TYPE_MIDDLE)[0];
	auto ring = fingers.fingerType(Finger::TYPE_RING)[0];
	auto pinky = fingers.fingerType(Finger::TYPE_PINKY)[0];
	auto thumb = fingers.fingerType(Finger::TYPE_THUMB)[0];
	return middle.isExtended() && ring.isExtended() && pinky.isExtended()
		&& !index.isExtended() && !thumb.isExtended();
}

bool isIndexFingerCurled(const Finger& frame)
{
	return !isIndexFingerStraight(frame);
}

bool areAllFingersStraight(const FingerList& fingers)
{
	auto num = fingers.count();
	for (int i = 0; i < num; ++i) {
		if (!fingers[i].isExtended()) return false;
	}
	return true;
}

bool areAllFingersCurled(const FingerList& fingers)
{
	auto num = fingers.count();
	for (int i = 0; i < num; ++i) {
		if (fingers[i].isExtended()) return false;
	}
	return num > 1;
}

bool isHandInDragShape(const FingerList& fingers)
{
	auto indexFinger = fingers.fingerType(Finger::TYPE_INDEX)[0];
	auto middleFinger = fingers.fingerType(Finger::TYPE_MIDDLE)[0];
	auto thumb = fingers.fingerType(Finger::TYPE_THUMB)[0];
	return indexFinger.isExtended() && middleFinger.isExtended() && thumb.isExtended();
}

void PadListener::onInit(const Controller& controller)
{
	std::cout << "Initialized." << std::endl;
	controller.setPolicy(Leap::Controller::POLICY_IMAGES);
}

void PadListener::onConnect(const Controller& controller)
{
	std::cout << "Connected." << std::endl;
	// we use SWIPE to indicate turning pages
	controller.enableGesture(Leap::Gesture::TYPE_SWIPE);
}

void PadListener::onDisconnect(const Controller& controller)
{
	std::cout << "Disconnected." << std::endl;
}

void PadListener::onExit(const Controller& controller)
{
	std::cout << "Exited." << std::endl;
}

void PadListener::onFocusGained(const Controller& controller)
{
	std::cout << "Focus gained." << std::endl;
}

void PadListener::onFocusLost(const Controller& controller)
{
	std::cout << "Focus lost." << std::endl;
}

void PadListener::onDeviceChange(const Controller& controller)
{
	std::cout << "!!!Device changed!!!" << std::endl;
}

void PadListener::onServiceConnect(const Controller& controller)
{
	std::cout << "Service connected." << std::endl;
}

void PadListener::onServiceDisconnect(const Controller& controller)
{
	std::cout << "Service disconnected." << std::endl;
}

Leap::Vector getPointerPosition(const Tool& pointer)
{
	return pointer.stabilizedTipPosition();
}

Leap::Vector getIndexFingerPosition(const Finger& index) {
	return index.stabilizedTipPosition()*3;
}

Leap::Vector getHandPosition(const FingerList& fingers) {
	unsigned num = fingers.count();
	Leap::Vector vec = fingers[0].stabilizedTipPosition();
	for (unsigned i = 1; i < num; ++i) {
		vec += fingers[i].stabilizedTipPosition();
	}
	return 2 * vec / num;
}

const Color colors[7] = {
	{ 0, 0, 0 },
	{ 255, 0, 0 },
	{ 0, 255, 0 },
	{ 0, 0, 255 },
	{ 255, 255, 0 },
	{ 0, 255, 255 },
	{ 255, 0, 255 }
};

Color getColorFor(int id) {
	return colors[sizeof(colors) % id];
}

/*
	This is where most program logic occurs.
*/
void PadListener::onFrame(const Controller& controller)
{
	const Leap::Frame frame = controller.frame();
	auto hands = frame.hands();
	// we only need to worry about 2 hands; 3 would be too crowded anyway
	auto hand1 = hands[0];
	auto fingers1 = hand1.fingers();
	auto hand2 = hands[1];
	auto indexFinger1 = fingers1.fingerType(Finger::TYPE_INDEX)[0]; // find the index finger

	if (areFingersInPictureShape(fingers1) && !camera) {
		std::cout << "Trying to take a picture..." << timer << " " << std::endl;

		if (timer >= PICTURE_TIME) {
			std::cout << "Ok!" << std::endl;
			time_t future = time(NULL) + 4;
			while (time(NULL) < future)
			{
			}
			camera = true;
			timer = 0;
		}
		timer++;
	}

	if (camera) {
		artist.drawImage(frame.images()[0]);

		if (areFingersInPictureShape(fingers1)) {
			std::cout << "Leaving picture mode..." << timer << " " << std::endl;

			if (timer >= PICTURE_TIME) {
				std::cout << "Ok!" << std::endl;
				camera = false;
				timer = 0;
			}
			timer++;
		}
	}
	
	if (areAllFingersCurled(fingers1)) { // if all fingers are curled, enter erase mode
		std::cout << "ERASE MODE" << std::endl;
		if (mode == ERASE && timer > ERASE_TIME) {
			artist.eraseAtPoint();
		}
		else {
			// We're not in drag mode, so we don't drag yet, but we do enter erase mode
			mode = ERASE;
			timer++;
		}
	}
	else if (isIndexFingerStraight(indexFinger1)) {
		Leap::Vector indexPos = getIndexFingerPosition(indexFinger1);
		std::cout << "DRAWING WITH FINGER" << std::endl;
		// If we're already in draw mode, we draw the line
		if (mode == DRAW) {
			artist.setPenColor(PLAYER1_COLOR);
			artist.drawLineBetween(indexPos.x, indexPos.y, lastX, lastY);
		}
		else {
			// If we're not in draw mode, we enter draw mode.
			// We do not connect the points because we only have one point so far.
			mode = DRAW;
		}
		// update the last points so that next time through we have the right points
		lastX = indexPos.x;
		lastY = -indexPos.y;
	}

	auto fingers2 = hand2.fingers();
	auto indexFinger2 = fingers2.fingerType(Finger::TYPE_INDEX)[0]; // find the index finger
	if (areAllFingersCurled(fingers2)) { // if all fingers are curled, enter erase mode
		std::cout << "ERASE MODE" << std::endl;
		if (mode2 == ERASE && timer2 > ERASE_TIME) {
			artist.eraseAtPoint();
		}
		else {
			// We're not in drag mode, so we don't drag yet, but we do enter erase mode
			mode2 = ERASE;
			timer2++;
		}
	}
	else if (isIndexFingerStraight(indexFinger2)) {
		Leap::Vector indexPos = getIndexFingerPosition(indexFinger2);
		std::cout << "DRAWING WITH FINGER" << std::endl;
		// If we're already in draw mode, we draw the line
		if (mode2 == DRAW) {
			artist.setPenColor(PLAYER2_COLOR);
			artist.drawLineBetween(indexPos.x, indexPos.y, lastX2, lastY2);
		}
		else {
			// If we're not in draw mode, we enter draw mode.
			// We do not connect the points because we only have one point so far.
			mode2 = DRAW;
		}
		// update the last points so that next time through we have the right points
		lastX2 = indexPos.x;
		lastY2 = -indexPos.y;
	}

	artist.display(display);
}
