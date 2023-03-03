#include <math.h>
#include <stdlib.h>
#include "raylib.h"

const int screenWidth = 600;
const int screenHeight = 600;

typedef struct Point{
	float x;
	float y;
}Point;

void PutPixel(float x, float y, Color c)
{
	float xx = screenWidth/2 +x;
	float yy = screenHeight/2 -y - 1;

	if (xx <0 || xx >= screenWidth || yy <0 || y >= screenHeight) {
		return;
	}
	DrawPixel((int)xx, (int)yy, c);
}

void swap(Point *p0, Point *p1)
{
	Point t;
	t.x = p0->x;
	t.y = p0->y;
	p0->x = p1->x;
	p0->y = p1->y;
	p1->x = t.x;
	p1->y = t.y;
}

void draw_line(Point *p0, Point *p1, Color c)
{
	if (p0->x > p1->y) {
		swap(p0, p1);
	}
	float a = ((p1->y - p0->y) / (p1->x - p0->x));
	float y = p0->y;
	for (int x = p0->x; x <= p1->x; x++)
	{
		PutPixel((int)x, (int)y, c);
		y = y + a;
	}

}

void draw_line2(Point *p0, Point *p1, Color c)
{
	float dx = p1->x - p0->x;
	float dy = p1->y - p0->y;

	if (abs(dx) > abs(dy)){
		// Line is horizontal-ish
		if (p0->x > p1->x) {
			swap(p0, p1);
		}
		float a = dy / dx;
		float y = p0->y;
		for (int x = p0->x; x <= p1->x; x++)
		{
			PutPixel((int)x, (int)y, c);
			y = y + a;
		}
	} else {
		// Line is vertical-ish
		if (p0->y > p1->y) {
			swap(p0, p1);
		}
		float a = dx / dy;
		float x = p0->x;
		for (int y = p0->y; y <= p1->y; y++)
		{
			PutPixel((int)x, (int)y, c);
			x = x + a;
		}
	}
}

int interpolate(float i0, float d0, float i1, float d1, float *values)
{
	int cnt = 0;
	if (i0 == i1){
		values[0] = d0;
		return 1;
	}
	float a = (d1 - d0) / (i1 - i0);
	float d = d0;
	for (int i = i0; i<=i1; i++){
		values[cnt++] = d;
		d = d + a;
	}
	return cnt;
}

void draw_line3(Point *p0, Point *p1, Color c)
{
	float dx = p1->x - p0->x;
	float dy = p1->y - p0->y;
	float yValues[screenHeight];
	float xValues[screenWidth];

	if (abs(dx) > abs(dy)){
		// Line is horizontal-ish
		if (p0->x > p1->x) {
			swap(p0, p1);
		}
		int len = interpolate(p0->x, p0->y, p1->x, p1->y, xValues);
		for (int x = p0->x; x <= p1->x; x++)
		{
			if (len > 1)
				PutPixel((int)x, (int)xValues[(int)(x - p0->x)], c);
			else 
				PutPixel((int)x, (int)xValues[0], c);
		}
	} else {
		// Line is vertical-ish
		if (p0->y > p1->y) {
			swap(p0, p1);
		}
		int len = interpolate(p0->y, p0->x, p1->y, p1->x, yValues);
		for (int y = p0->y; y <= p1->y; y++)
		{
			if (len > 1)
				PutPixel((int)xValues[(int)(y - p0->y)], (int)y, c);
			else 
				PutPixel((int)xValues[0], (int)y, c);
		}
	}
}

//------------------------------------------------------------------------------------------
// Types and Structures Definition
//------------------------------------------------------------------------------------------
typedef enum Screen { LOGO = 0, SCREEN_1, SCREEN_2, SCREEN_3, SCREEN_4, ENDING } Screen;

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    Screen currentScreen = LOGO;

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
	// Update
        //----------------------------------------------------------------------------------
        switch(currentScreen)
        {
            case LOGO:
            {
                if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
                {
                    currentScreen = SCREEN_1;
                }
            }
            break;
	    case SCREEN_1:
            {
                // Press enter to change to screen
                if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
                {
                    currentScreen = SCREEN_2;
                }
            } break;
            case SCREEN_2:
            {
                // Press enter to change to screen
                if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
                {
                    currentScreen = SCREEN_3;
                }
            } break;
            case SCREEN_3:
            {
                // Press enter to change to screen
                if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
                {
                    currentScreen = SCREEN_4;
                }
            } break;
            case SCREEN_4:
            {
                // Press enter to change to screen
                if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
                {
                    currentScreen = ENDING;
                }
            } break;

            case ENDING:
            {
                // Press enter to return to TITLE screen
                if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
                {
                    currentScreen = SCREEN_1;
                }
            } break;
            default: break;
        }

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
 	switch(currentScreen)
            {
                case LOGO:
                {
                    // TODO: Draw LOGO screen here!
                    DrawText("PRESS ENTER or TAP to JUMP to SCREEN", 120, 220, 20, DARKGREEN);

                } break;
                case SCREEN_1:
                {
			Point p0 = {-200, -100};
	    		Point p1 = {240, 120};
	    		draw_line(&p0, &p1, BLACK);
                } break;
                case SCREEN_2:
                {
			Point p0 = {-50, -200};
	    		Point p1 = {60, 240};
	    		draw_line(&p0, &p1, BLACK);
                } break;
		case SCREEN_3:
                {
			Point p0 = {-200, -100};
	    		Point p1 = {240, 120};
	    		draw_line2(&p0, &p1, BLACK);
		    	Point p2 = {-50, -200};
		    	Point p3 = {60, 240};
		    	draw_line2(&p2, &p3, BLACK);
                }
		break;
		case SCREEN_4:
                {
			Point p0 = {-200, -100};
	    		Point p1 = {240, 120};
	    		draw_line3(&p0, &p1, BLACK);
		    	Point p2 = {-50, -200};
		    	Point p3 = {60, 240};
		    	draw_line3(&p2, &p3, BLACK);
                } break;
  break;
                case ENDING:
                {
                    // TODO: Draw ENDING screen here!
                    DrawText("PRESS ENTER or TAP to RETURN to TITLE SCREEN", 120, 220, 20, DARKBLUE);

                } break;
                default: break;
            }
	
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
