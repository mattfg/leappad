#pragma once

// Constants.h

#define ERASER_RADIUS 32

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

#define CANVAS_MAX_X WINDOW_WIDTH
#define CANVAS_MAX_Y WINDOW_HEIGHT

#define CANVAS_COLOR {250, 250, 250}

#define BLACK {0, 0, 0}
#define BLUE {0, 0, 255}
#define RED {255, 0, 255}
#define GREEN {0, 255, 0}

#define PLAYER1_COLOR RED
#define PLAYER2_COLOR GREEN

#define ERASE_TIME 120
#define DRAG_TIME 80
#define DRAW_TIME 0
#define PICTURE_TIME 120

#define PICTURE_DISTANCE 125

enum DrawMode {
	NONE, DRAW, DRAG, ERASE
};
