/*******************************************************************************************
*
*   raylib [core] example - Basic window
*
*   Welcome to raylib!
*
*   To test examples, just press F6 and execute raylib_compile_execute script
*   Note that compiled executable is placed in the same folder as .c file
*
*   You can find all basic examples on C:\raylib\raylib\examples folder or
*   raylib official webpage: www.raylib.com
*
*   Enjoy using raylib. :)
*
*   Example originally created with raylib 1.0, last time updated with raylib 1.0
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2013-2023 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include <math.h>
#include <stdlib.h>
#include "raylib.h"

const int screenWidth = 800;
const int screenHeight = 800;

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
		if (dx < 0) {
			swap(p0, p1);
		}
		int len = interpolate(p0->x, p0->y, p1->x, p1->y, xValues);
		printf("len %d\n", len);
		for (int k=0; k<20; k++)
		{
			//printf("%f ", xValues[k]);
		}
		printf("\n");
		//float a = dy / dx;
		//float y = p0->y;
		for (int x = p0->x; x <= p1->x; x++)
		{
			//printf("%f ", xValues[(int)(x - p0->x)]);
			//exit(0);
			if (len > 1)
				PutPixel((int)x, (int)xValues[((int)x - (int)p0->x)], c);
			else 
				PutPixel((int)x, (int)xValues[0], c);
			//y = y + a;
		}
	} else {
		// Line is vertical-ish
		if (dy < 0) {
			swap(p0, p1);
		}
		//float a = dx / dy;
		//float x = p0->x;
		int len = interpolate(p0->y, p0->x, p1->y, p1->x, yValues);
		for (int y = p0->y; y <= p1->y; y++)
		{
			if (len > 1)
				PutPixel((int)xValues[((int)y - (int)p0->y)], (int)y, c);
			else 
				PutPixel((int)xValues[0], (int)y, c);
			//x = x + a;
		}
	}
}

void DrawWireframeTriangle(Point *p0, Point *p1, Point *p2)
{
	draw_line3(p0, p1, BLACK);
	draw_line3(p1, p2, BLACK);
	draw_line3(p2, p0, BLACK);
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
#if 1
	Point p0 = {-200, -250};
        Point p1 = {200, 50};
        Point p2 = {20, 250};
	//draw_line3(&p0, &p1, BLACK);
//	DrawWireframeTriangle(&p0, &p1, &p2);
	//draw_line2(&p0, &p1, BLACK);
	//draw_line3(&p0, &p1, BLACK);
	draw_line2(&p1, &p2, BLACK);
	draw_line3(&p1, &p2, BLACK);
	//draw_line2(&p2, &p0, BLACK);
#endif

#if 0
	    Point p0 = {-200, -100};
	    Point p1 = {240, 120};
	    draw_line3(&p0, &p1, BLACK);
	    
	    Point p2 = {-50, -200};
	    Point p3 = {60, 240};
	    draw_line3(&p2, &p3, BLACK);
#endif
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
