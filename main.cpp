/*
	This program uses the following libraries and APIS:
		cimg library (http://cimg.sourceforge.net)
		Leap Motion SDK (http://developer.leapmotion.com)
*/

#include "include/Leap.h"
#include "include/CImg.h"
#include "PadListener.h"
#include "constants.h"

/*
	Thanks to Leap Motion for a sample implementation of main.
	Most program logic is in PadListener.cpp and PadArtist.cpp
	Special thanks to Mateen Ulhaq at StackOverflow for snippets for mouse control
*/
int main(int argc, char** args)
{
	// Create a sample listener and controller
	cimg_library::CImg<unsigned char> image (CANVAS_MAX_X, CANVAS_MAX_Y, 1, 3);
	cimg_library::CImgDisplay display (WINDOW_WIDTH, WINDOW_HEIGHT, "LeapPad");
	//display.render(image);
	//display.set_fullscreen(true);
	PadListener listener(image, display); // Our program
	Leap::Controller controller; // the Leap Motion device

	// Have the sample listener receive events from the controller
	controller.addListener(listener);

	if (argc > 1 && strcmp(args[1], "--bg") == 0)
		controller.setPolicy(Leap::Controller::POLICY_BACKGROUND_FRAMES);

	// Keep this process running until Enter is pressed
	std::cout << "Press Enter to quit..." << std::endl;
	std::cin.get();

	// Remove the sample listener when done
	controller.removeListener(listener);
	return 0;
}