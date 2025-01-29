
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#define SDL_MAIN_HANDLED
#include "SDL2/include/SDL.h"

uint16_t *pixels;


#define WIDTH 900
#define HEIGHT 500

#define GAMMA 1
#define FAR 10000

using namespace std;

SDL_Window* screen;
SDL_Renderer* renderer;
SDL_Texture* pixBuf;


class Vec3 {
public:
    Vec3(float x = 0, float y = 0, float z = 0) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    Vec3 operator-(Vec3 b) {
        return Vec3(this->x - b.x, this->y - b.y, this->z - b.z);
    }
    Vec3 operator+(Vec3 b) {
        return Vec3(this->x + b.x, this->y + b.y, this->z + b.z);
    }
    void operator+=(Vec3 b) {
        this->x += b.x;
        this->y += b.y;
        this->z += b.z;
    }

    Vec3 operator*(Vec3 b) {
        return Vec3(this->x * b.x, this->y * b.y, this->z * b.z);
    }

    Vec3 operator*(float b) {
        return Vec3(this->x * b, this->y * b, this->z * b);
    }

    Vec3 operator/(Vec3 b) {
        return Vec3(this->x / b.x, this->y / b.y, this->z / b.z);
    }

    Vec3 operator/=(float b) {
        this->x /= b;
        this->y /= b;
        this->z /= b;
        return *this;
    }

    bool operator==(Vec3 b) {
        return this->x == b.x && this->y == b.y && this->z == b.z;
    }


    Vec3 Vabs() {
        return Vec3(fabs(this->x), fabs(this->y), fabs(this->z));
    }

    float len() {
        return sqrt(this->len2());
    }

    float len2() {
        return this->x * this->x + this->y * this->y + this->z * this->z;
    }

    void normalize() {
        float l = this->len();
        this->x /= l;
        this->y /= l;
        this->z /= l;
    }



    float x;
    float y;
    float z;
};


Vec3 BLACK = {0, 0, 0};
Vec3 LIGHT = {1, 1, 1};
Vec3 ambient = {0.1, 0.1, 0.1};
float fov = M_PI/3;
float d = tan(fov/2);
float thr = 0.001;
float ratio = (float)WIDTH/HEIGHT;

int antialiasing = 0;
bool sky = false;

float det(Vec3 u, Vec3 v) {
    return u.x*v.x + u.y*v.y + u.z*v.z;
}

Vec3 nullVec3 = Vec3(-1, -1, -1);
class Obj {
    public:
    Obj(float alpha, float metal, Vec3 alb = nullVec3) {
        _type = 0;
        _alb = alb;
        _alpha = alpha;
        _metal = metal;
    }

    Obj(int type, Vec3 pos, Vec3 alb, float s, float alpha, float metal) {
        _type = type;
        _pos = pos;
        _alb = alb;
        _s = s; // size, or radius
        _alpha = alpha;
        _metal = metal;
    }
    
    Vec3 alb(Vec3 pos) {
        if (_type == 0 && _alb == nullVec3) {
            float c = ((int)(floor(pos.x/4.0f)) & 1) ^ ((int)(floor(pos.z/4.0f)) & 1);
            return {c, 1.0f-c, 0};
        }
        return _alb;
    }

    // used for cubes
    pair<int,float> getFace(Vec3 pos) {
        Vec3 diff = pos-_pos;
        float dx = abs(diff.x), dy = abs(diff.y), dz = abs(diff.z);
        if (dx > dy) {
            if (dx > dz) {
                return {0, dx};
            }
            return {2, dz};
        }
        if (dy > dz) {
            return {1, dy};
        }
        return {2, dz};
    }

    float dist(Vec3 pos) {
        switch(_type) {
            case 0: return abs(pos.y);
            case 1: return getFace(pos).second-_s;
            case 2: return (_pos-pos).len()-_s;
        }
        throw;
    }

    Vec3 reflect(Vec3 pos, Vec3 mov) {
        switch(_type) {
            case 0: return Vec3(mov.x, -mov.y, mov.z);
            case 1: {
                int i = getFace(pos).first;
                if (i == 0) return Vec3(-mov.x, mov.y, mov.z);
                if (i == 1) return Vec3(mov.x, -mov.y, mov.z);
                return Vec3(mov.x, mov.y, -mov.z);
            }
            case 2: {
                Vec3 diff = pos-_pos;
                diff.normalize();
                return diff * (det(mov, diff)*-2) + mov;
            }
        }
        throw;
    }

    Vec3 normal(Vec3 pos) {
        switch(_type) {
            case 0: return Vec3(0, 1, 0);
            case 1: {
                Vec3 diff = pos-_pos;
                int i = getFace(pos).first;
                if (i == 0) return Vec3(diff.x < 0 ? -1 : 1, 0, 0);
                if (i == 1) return Vec3(0, diff.y < 0 ? -1 : 1, 0);
                return Vec3(0, 0, diff.z < 0 ? -1 : 1);
            }
            case 2: {
                Vec3 diff = pos-_pos;
                diff.normalize();
                return diff;
            }
        }
        throw;
    }

    int _type; // 0: floor, 1: cube, 2: ball
    Vec3 _pos;
    Vec3 _alb;
    float _s;
    float _alpha;
    float _metal;
};

// sphere
/* Vec3 light = {-2, 5, 10};
Vec3 cam = Vec3(0, 5, 20);
float rot[] = {0, 0}; */
// cube and sphere
Vec3 light = {7, 5, 10};
Vec3 cam = Vec3(25, 5, 15);
float rot[] = {0, M_PI/4};
// infinite mirrors
/* Vec3 light = {0, 5, 0};
Vec3 cam = Vec3(0, 5, 0);
float rot[] = {0, 0}; */

/* Obj ball = Obj(2, Vec3(0, 5, 0), {1, 1, 1}, 5, 1, 1);
Obj flr = Obj(1, 1);
Obj* obj[] = {&ball, &flr}; */
Obj cube = Obj(1, Vec3(0, 5, 0), {1, 1, 1}, 4, 1, 0);
Obj ball = Obj(2, Vec3(8, 5, -7), {1, 0.9, 0}, 5, 1, 1);
Obj flr = Obj(1, 1);
Obj* obj[] = {&cube, &ball, &flr};
/* Obj cube1 = Obj(1, Vec3(0, 5, -20), {0.6, 0.8, 0.8}, 5, 0.1, 1);
Obj cube2 = Obj(1, Vec3(0, 5, 10), {0.6, 0.8, 0.8}, 5, 0.1, 1);
Obj ball1 = Obj(2, Vec3(-8, 5, -5), {0, 0, 0}, 5, 0, 0.7);
Obj ball2 = Obj(2, Vec3(8, 5, -5), {0, 0, 0}, 5, 0, 0.7);
Obj flr = Obj(1, 0.5, {1, 0.7, 0});
Obj* obj[] = {&cube1, &cube2, &ball1, &ball2, &flr}; */

float cos0 = cos(rot[0]), sin0 = sin(rot[0]), cos1 = cos(rot[1]), sin1 = sin(rot[1]);

int N = 7;
int s = 1;
int x;
int y;

float min2(float a, float b) {
    return a < b ? a : b;
}

float max2(float a, float b) {
    return a > b ? a : b;
}

Vec3 lighting(Vec3 pos, Obj* o, Vec3 col, float dif, float spec) {
    float t1 = o->_alpha;
    float t2 = o->_metal;
    Vec3 alb = o->alb(pos);
    float difr = dif, difg = dif, difb = dif;
    if (difr < ambient.x) difr = ambient.x;
    if (difg < ambient.y) difg = ambient.y;
    if (difb < ambient.z) difb = ambient.z;
    if (spec < 0) spec = 0;
    return {min2(col.x*t2 + (alb.x*difr*t1+spec*t2) * LIGHT.x, 1),
            min2(col.y*t2 + (alb.y*difg*t1+spec*t2) * LIGHT.y, 1),
            min2(col.z*t2 + (alb.z*difb*t1+spec*t2) * LIGHT.z, 1)};
}

Vec3 postPro(Vec3 pos, Vec3 col) {
    return col;
}

pair<float,Obj*> getDist(Vec3 pos) {
    float d2 = -1;
    Obj* _o;
    for (Obj* o : obj) {
        float d = o->dist(pos);
        if (d < 0) d = 0;
        if (d2 == -1 or d < d2) d2 = d, _o = o;
    }
    return {d2, _o};
}

Vec3 background() {
    float t = (float)y/HEIGHT*2;
    if (t > 1) t = 1;
    return Vec3(0.5 + 0.3*t, 0.5 + 0.5*t, 1);
}

Vec3 ray(Vec3 pos, Vec3 mov, int n) {
    Vec3 col = sky? background() : BLACK;
    if (n < 0) return col;

    float prevd = 0;
    float dist;
    Obj* o;

    float minD = FAR;
    while (true) {
        pair<float,Obj*> p = getDist(pos);
        dist = p.first, o = p.second;
        if (dist < minD) minD = dist;
        if (dist <= prevd && dist < thr) break;
        prevd = dist;
        if ((pos-cam).len2() > FAR) break;
        pos += mov * (dist*0.99);
    }

    if (dist < thr) {
        Vec3 newmov = o->reflect(pos, mov);
        Vec3 reflected = ray(Vec3(pos), newmov, n-1);
        Vec3 u = light-pos;
        Vec3 v = mov;
        u.normalize();
        v.normalize();
        col = lighting(pos, o, reflected, det(u, o->normal(pos)), det(u, v));
    }

    if (n == N) return postPro(pos, col);
    return col;
}

void refresh() {
    // warning: costly operation
    SDL_UpdateTexture(pixBuf, NULL, pixels, WIDTH*sizeof(Uint32));
    SDL_RenderCopy(renderer, pixBuf, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void draw_sqr(int x, int y, Vec3 col) {
    
    float m = 255*GAMMA;
    int r = 31*col.z, g = 63*col.y, b = 31*col.x;
    uint16_t color = (b << 11) | (g << 5) | r;
    if (s == 1) pixels[y*WIDTH + x] = color;

    else {
        for (int j = x; j < x+s && j < WIDTH; j++)
            for (int i = y; i < y+s && i < HEIGHT; i++)
                pixels[i*WIDTH + j] = color;
    }
}

Vec3 getMov(float x, float y) {
    // fisheye, supports >180° fov
    // float a1 = rot[0] + tan((y - (HEIGHT>>1))*d), a2 = rot[1] + tan((x - (WIDTH>>1))*d);
    // Vec3 mov = {cos(a1)*sin(a2), -sin(a1), -cos(a1)*cos(a2)};

    float dx = d * ratio * (x/WIDTH - 0.5f), dy = -d * (y/HEIGHT - 0.5f), dz = -1;
    // rotate ray according to camera
    Vec3 mov = {
        dx*cos1 + dy*sin0*sin1 + dz*cos0*sin1,
        dy*cos0 - dz*sin0,
        -dx*sin1 + dy*sin0*cos1 + dz*cos0*cos1
    };
    mov.normalize();
    return mov;
}

void pixel(int x, int y) {
    Vec3 col;
    if (antialiasing > 1) {
        col = Vec3();
        for (int i = 0; i < antialiasing; i++)
            for (int j = 0; j < antialiasing; j++)
                col += ray(cam, getMov(x + (float)i/antialiasing, y + (float)j/antialiasing), N);
        col /= (float)antialiasing*antialiasing;
    }
    else col = ray(cam, getMov(x, y), N);

    draw_sqr(x, y, col);
}


int RayMain(void){
    for (int32_t y = 0; y < HEIGHT; y += s) {
        for (int32_t x = 0; x < WIDTH; x += s) {
            pixel(x, y);
        }
    }
    return 0;
}


int main(int argc, char **argv)
{
    printf("Hello World\n");
    SDL_Init(SDL_INIT_VIDEO);

    screen = SDL_CreateWindow("SDL2 Pixel Drawing",
                                          SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, 0);

    renderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_ACCELERATED);

    pixBuf = SDL_CreateTexture(renderer,
                                             SDL_PIXELFORMAT_RGB565, SDL_TEXTUREACCESS_STATIC, WIDTH, HEIGHT);

    pixels = (uint16_t *)malloc(WIDTH * HEIGHT * sizeof(uint16_t));

    for (int i = 0; i < WIDTH * HEIGHT; i++)
    {
        pixels[i] = 0;
    }

    RayMain();
    SDL_UpdateTexture(pixBuf, NULL, pixels, WIDTH * sizeof(uint16_t));

    SDL_Event event;
    int quit = 0;

    while (!quit)
    {
        SDL_UpdateTexture(pixBuf, NULL, pixels, WIDTH * sizeof(uint16_t));
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, pixBuf, NULL, NULL);
        SDL_RenderPresent(renderer);

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                quit = 1;
            }
        }
    }

    free(pixels);
    SDL_DestroyTexture(pixBuf);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(screen);
    SDL_Quit();

    return 0;
}