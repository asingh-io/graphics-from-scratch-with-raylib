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

typedef struct Sphere{
    VectorPoint center;
    int radius;
    Color color;
}Sphere;

typedef struct Light{
    int type;
    float intensity;
    VectorPoint position;
    VectorPoint direction;
}Light;

Sphere spheres[] = {
    { {0, -1, 3}, 1, {255, 0, 0, 255} }, // RED sphere
    { {2, 0, 4}, 1, {0, 0, 255, 255} },  // GREEN sphere
    { {-2, 0, 4}, 1, {0, 255, 0, 255} }, // BLUE sphere
    { {0, -5001, 0}, 5000, {255, 255, 0, 255} }, // BLUE sphere
};

enum {
    AMBIENT = 0,
    POINT = 1,
    DIRECTION = 2,
    MAX_LIGHT = 3
};

Light light[] = {
    {AMBIENT, 0.2, {}, {}},
    {POINT, 0.6, {2,1,0}, {}},
    {DIRECTION, 0.2, {}, {1,4,4}},
};


void print_vector(char *str, VectorPoint *v)
{
    printf("%s : {%f, %f, %f}\n", str, v->x, v->y, v->z);

}
/*
AmbientLight ambient_light = {0.2};
PointLight point_light = {0.6, {2, 1, 0}};
DirectionalLight direction_light = { 0.2, {1, 4, 4}};
*/

#define SPHERES_COUNT (sizeof(spheres)/sizeof(spheres[0]))

float dotProduct(VectorPoint *p1, VectorPoint *p2)
{
    return ((p1->x * p2->x) + (p1->y * p2->y) + (p1->z * p2->z));
}

float vectorLength(VectorPoint *v)
{
    return sqrt(dotProduct(v, v));
}

void multiply(float k, VectorPoint *p, VectorPoint *result)
{
    result->x = p->x * k;
    result->y = p->y * k;
    result->z = p->z * k;
}

void add(VectorPoint *p1, VectorPoint *p2, VectorPoint *result)
{
    result->x = p1->x + p2->x;
    result->y = p1->y + p2->y;
    result->z = p1->z + p2->z;
}

void subtract(VectorPoint *p1, VectorPoint *p2, VectorPoint *result)
{
    result->x = p1->x - p2->x;
    result->y = p1->y - p2->y;
    result->z = p1->z - p2->z;
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

float ComputeLighting(VectorPoint *point, VectorPoint *normal)
{
  float intensity = 0;
  int length_n = vectorLength(normal);  // Should be 1.0, but just in case...

  for (int i = 0; i < MAX_LIGHT; i++) {
   // Light light = lights[i];
    if (light[i].type == AMBIENT) {
      intensity += light[i].intensity;
    } else {
      VectorPoint vec_l;
      if (light[i].type == POINT) {
	    subtract(&light[i].position, point, &vec_l);
      } else {  // Light.DIRECTIONAL
	    vec_l.x = light[i].position.x;
	    vec_l.y = light[i].position.y;
	    vec_l.z = light[i].position.z;
      }

      float n_dot_l = dotProduct(normal, &vec_l);
      if (n_dot_l > 0) {
        	intensity += light[i].intensity * n_dot_l / (length_n * vectorLength(&vec_l));
      }
    }
  }

  return intensity;

}
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define MAX(x, y) (((x) > (y)) ? (x) : (y))

unsigned char clamp(unsigned char color)
{
    return MIN(255, MAX(0, color));
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
        }
        if (p.t2 > t_min && p.t2 < t_max && p.t2 < closet_t) {
            closet_t = p.t2;
            closet_sphere = &spheres[i];
        }
    }
    if (closet_sphere == NULL) {
         return;
    }

    /*  adding light */
    VectorPoint tmp;
    VectorPoint point, normal;

    multiply(closet_t, D, &tmp);

    add(O, &tmp, &point);
    subtract(&point, &closet_sphere->center, &tmp);

    multiply((1.0 / vectorLength(&tmp)), &tmp, &normal);

   float light = ComputeLighting(&point, &normal);

   color->r = clamp(light * closet_sphere->color.r);
   color->g = clamp(light * closet_sphere->color.g);
   color->b = clamp(light * closet_sphere->color.b);
   color->a = 255;

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
    InitWindow(screenWidth, screenHeight, "chap2: raytracing");

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
