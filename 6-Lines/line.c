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
	//float b = p0->y - (a * p0->x);
	for (int x = p0->x; x <= p1->x; x++)
	{
	//	float y = (a * x) + b;
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
		//float a = dy / dx;
		//float y = p0->y;
		for (int x = p0->x; x <= p1->x; x++)
		{
			if (len > 1)
				PutPixel((int)x, (int)xValues[(int)(x - p0->x)], c);
			else 
				PutPixel((int)x, (int)xValues[0], c);
			//y = y + a;
		}
	} else {
		// Line is vertical-ish
		if (p0->y > p1->y) {
			swap(p0, p1);
		}
		//float a = dx / dy;
		//float x = p0->x;
		int len = interpolate(p0->y, p0->x, p1->y, p1->x, yValues);
		for (int y = p0->y; y <= p1->y; y++)
		{
			if (len > 1)
				PutPixel((int)xValues[(int)(y - p0->y)], (int)y, c);
			else 
				PutPixel((int)xValues[0], (int)y, c);
			//x = x + a;
		}
	}
}


//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
	    Point p0 = {-200, -100};
	    Point p1 = {240, 120};
	    draw_line3(&p0, &p1, BLACK);
	    
	    Point p2 = {-50, -200};
	    Point p3 = {60, 240};
	    draw_line3(&p2, &p3, BLACK);
	   /* 
	   /* 
	    for ( int x = -240; x <240; x++){
		    for ( int y = -100; y<120; y++) {
			    PutPixel(x,y, BLACK);
		    }
	    }
	    */

//            DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
