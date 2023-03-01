#include <math.h>
#include "raylib.h"

const int screenWidth = 800;
const int screenHeight = 800;

void PutPixel(int x , int y, Color color)
{
    int sx = screenWidth /2 + x;
    int sy = screenHeight /2 - y;
    DrawPixel(sx, sy, color);

}

void DrawSpiral()
{
    float angle = 0;
    float radius = 1;
    while(radius < screenWidth/2){
        float y = radius * sin(angle);
        float x = radius * cos(angle);
        PutPixel((int)x, (int)y, RED);
        radius += 0.005;
        angle += 3.14159f * .0005f;
    }
}

int main(void)
{
    InitWindow(screenWidth, screenHeight, "raylib example - basic window");

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawSpiral();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
