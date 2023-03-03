#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "raylib.h"

const int screenWidth = 600;
const int screenHeight = 600;

const int viewPortWidth = 1;
const int viewPortHeight = 1;
const int viewPortDistance = 1;

void PutPixel(int x , int y, Color color)
{
    int sx = screenWidth /2 + x;
    int sy = screenHeight /2 - y;
    DrawPixel(sx, sy, color);

}

typedef struct Point{
    float t1;
    float t2;
}Point;

typedef struct VectorPoint{
    float x;
    float y;
    float z;
}VectorPoint;

typedef struct Center {
    int x;
    int y;
    int z;
}Center;

typedef struct Sphere{
    Center center;
    int radius;
    Color color;
}Sphere;


Sphere spheres[] = {
    { {0, -1, 3}, 1, RED }, // RED sphere
    { {2, 0, 4}, 1, GREEN },  // GREEN sphere
    { {-2, 0, 4}, 1, BLUE }, // BLUE sphere
};

#define SPHERES_COUNT (sizeof(spheres)/sizeof(spheres[0]))

float dotProduct(VectorPoint *p1, VectorPoint *p2)
{
    return ((p1->x * p2->x) + (p1->y * p2->y) + (p1->z * p2->z));
}


 void IntersectRaySphere(VectorPoint *origin, VectorPoint *direction, Sphere *sphere, Point *p)
{
    int r = sphere->radius;
    VectorPoint CO;
    CO.x = origin->x - sphere->center.x;
    CO.y = origin->y - sphere->center.y;
    CO.z = origin->z - sphere->center.z;

    float a = dotProduct(direction, direction);
    float b = 2*dotProduct(&CO, direction);
    float c = dotProduct(&CO, &CO) - (float)r*r;

    float discriminant = b*b - 4*a*c;

    if (discriminant < 0) {
        p->t1 = INFINITY;
        p->t2 = INFINITY;
        return;
    }

    p->t1 = (-b + sqrt(discriminant)) / (2*a);
    p->t2 = (-b - sqrt(discriminant)) / (2*a);
}


VectorPoint *CanvasToViewPort(int x, int y)
{
   VectorPoint *p = (VectorPoint *)malloc(sizeof(VectorPoint));
   p->x = (float)(x*viewPortWidth) / (float)screenWidth;
   p->y = (float)(y*viewPortHeight) / (float)screenHeight;
   p->z = (float)viewPortDistance;
   return p;
}

void TraceRay(VectorPoint *O, VectorPoint *D, float t_min, float t_max, Color *color)
{
    float closet_t = INFINITY;
    Sphere *closet_sphere = NULL;
    Point p;


    for (int i=0; i< SPHERES_COUNT; i++) {
        IntersectRaySphere(O, D, &spheres[i], &p);
        if (p.t1 > t_min && p.t1 < t_max && p.t1 < closet_t) {
            closet_t = p.t1;
            closet_sphere = &spheres[i];
            *color = closet_sphere->color;
        }
        if (p.t2 > t_min && p.t2 < t_max && p.t2 < closet_t) {
            closet_t = p.t2;
            closet_sphere = &spheres[i];
        }
    }
    if (closet_sphere == NULL) {
         return;
    }
    *color = closet_sphere->color;
}

void RayTracing()
{
    /*
     * 1. Place the camera and the viewport as desired
     * for each pixel on the canvas
     *      2. Determine which square on the viewport corresponds to this pixel
     *      3. Determine the color seen through that square
     *      4. Paint the pixel with that color
     */
    VectorPoint *O = (VectorPoint *)malloc(sizeof(VectorPoint));
    memset((VectorPoint *)O, 0, sizeof(VectorPoint));
    for (int x = -screenWidth/2; x <= screenWidth/2; x++)
    {
        for (int y = -screenHeight/2; y <= screenHeight/2; y++)
        {
            VectorPoint *D = CanvasToViewPort(x, y);
            Color color = WHITE;
            TraceRay(O, D, 1.0, INFINITY, &color);
            PutPixel((int)x,(int)y, color);
            free(D);
        }
    }
    free(O);
}

int main(void)
{
    InitWindow(screenWidth, screenHeight, "chap1: raytracing");

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        RayTracing();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
