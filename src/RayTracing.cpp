#include "BaseLib/baseLib.h"
#include "BaseLib/OS.h"
#include "BaseLib/FileSystem.h"
#include "BaseLib/memory.h"
#include "BaseLib/list.h"
#include "RayTracing.h"

#include <stdlib.h>
#include <stdio.h>

#include <math.h>
//#define RAPH_RTX
#define LEO_RTX

#ifdef RAPH_RTX

const float ambient = 0.1;
const float thr = 0.01;

float fov = 3.141592 / 2;
float _d = atan(fov / 2) / 64;

const float rot0 = 0;
const float rot1 = -3.141592 / 4;

const int32_t N = 16;

const float RD2 = 10000;


class Vector3 {
public:
    Vector3(float x = 0, float y = 0, float z = 0) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    Vector3 operator-(Vector3 b) {
        return Vector3(this->x - b.x, this->y - b.y, this->z - b.z);
    }
    Vector3 operator+(Vector3 b) {
        return Vector3(this->x + b.x, this->y + b.y, this->z + b.z);
    }
    void operator+=(Vector3 b) {
        this->x += b.x;
        this->y += b.y;
        this->z += b.z;
    }

    Vector3 operator*(Vector3 b) {
        return Vector3(this->x * b.x, this->y * b.y, this->z * b.z);
    }

    Vector3 operator*(float b) {
        return Vector3(this->x * b, this->y * b, this->z * b);
    }

    Vector3 operator/(Vector3 b) {
        return Vector3(this->x / b.x, this->y / b.y, this->z / b.z);
    }


    Vector3 Vabs() {
        return Vector3(fabs(this->x), fabs(this->y), fabs(this->z));
    }

    float length() {
        return sqrt(this->length2());
    }

    float length2() {
        return this->x * this->x + this->y * this->y + this->z * this->z;
    }

    void normalize_ip() {
        float l = this->length();
        this->x /= l;
        this->y /= l;
        this->z /= l;
    }


        
    float x;
    float y;
    float z;
};

class Obj {
public:
    Obj(Vector3 pos, Vector3 _alb, float refl) {
        this->pos = pos;
        this->_alb = _alb;
        this->refl = refl;
    }

    Vector3 alb() {
        return _alb;
    }

    Vector3 pos;
    Vector3 _alb;
    float refl;
};

class Floor {
public:
    Floor(float refl) { 
        this->refl = refl; 
    }
    Vector3 alb(Vector3 pos) {
        float c = ((int32_t)floor(pos.x / 4) & 1) ^ ((int32_t)floor(pos.z / 4) & 1);
        return Vector3(c, 1 - c, 0);
    }
    float dist(Vector3 pos) {
        return fabs(pos.y);
    }
    
    Vector3 reflect(Vector3 pos, Vector3 mov) {
            return Vector3(mov.x, -mov.y, mov.z);
    }

    Vector3 normal(Vector3 pos) 
    {
        return Vector3(0, 1, 0);
    }
    float refl;
};

class face{
public:
    face(int32_t n = 0, float d = 0) { this->n = n; this->d = d;}
    int32_t n;
    float d;
};

class Cube : public Obj {
public:
    Cube(float s, Vector3 pos, Vector3 alb, float refl) : Obj(pos, alb, refl) {
        this->s = s;
    }

    face getface(Vector3 pos) {
        Vector3 sub = pos - this->pos;
        Vector3 d = sub.Vabs();
        if (d.x > d.y) {
            if (d.x > d.z) return face(0, d.x);
            return face(2, d.z);
        }
        if (d.y > d.z) return face(1, d.y);
        return face(2, d.z);
    }
    float dist(Vector3 pos) {
        float d = this->getface(pos).d;
        float all = d - this->s;
        return all;
    }
    Vector3 reflect(Vector3 pos, Vector3 mov) {
        int32_t i = this->getface(pos).n;
        if (i == 0) return Vector3(-mov.x, mov.y, mov.z);
        if (i == 1) return Vector3(mov.x, -mov.y, mov.z);
        return Vector3(mov.x, mov.y, -mov.z);
    }
    Vector3 normal(Vector3 pos) {
        Vector3 a = pos - this->pos;
        int32_t i = this->getface(pos).n;
        if (i == 0) return Vector3(a.x < 0 ? -1 : 1, 0, 0);
        if (i == 1) return Vector3(0, a.y < 0 ? -1 : 1, 0);
        return Vector3(0, 0, a.z < 0 ? -1 : 1);
    }
    float s;
};


Vector3 cam = Vector3(20, 5, 20);

Vector3 LIGHT = Vector3(1, 1, 1);
Vector3 light = Vector3(-2, 5, 10);
Vector3 B = Vector3(0.2, 0.2, 0.8);

Floor fl = Floor(0);

Cube cb = Cube(5, Vector3(0, 5, 0), Vector3(1, 1, 1), 1);


float det(Vector3 u, Vector3 v) { return u.x * v.x + u.y * v.y + u.z * v.z; }

Vector3 lighting(Vector3 pos, int32_t n, Vector3 c, float d, float s) {
    //c : reflection, d : diffuse, s : specular

    Vector3 a = n == 0 ? fl.alb(pos) : cb.alb();
    float t = n == 0 ? fl.refl : cb.refl;

    if (d < ambient)  d = ambient;
    if (s < 0) s = 0;
    Vector3 res = Vector3(fmin((t * c.x) + a.x * d + s * LIGHT.x, 1), fmin((t * c.y) + a.y * d + s * LIGHT.y, 1), fmin(t * c.z + a.z * d + s * LIGHT.z, 1));
    return res;

}
   
Vector3 postpro(Vector3 pos, Vector3 col) {
    return col;
}

face get_dist(Vector3 pos) {
    float d2 = -6969;
    int32_t type = -1;
    float d = fl.dist(pos);
    if (d <= 0) d = 0.001;
    if ((d2 == -6969) || (d < d2)) {
        d2 = d;
        type = 0;
    }
    d = cb.dist(pos);
    if (d <= 0) d = 0.001;
    if ((d2 == -6969) || (d < d2)) {
        d2 = d;
        type = 1;
    }
    if(d == 0) d = 0.001;
    return face(type, d2);
}


int32_t count = 0;

Vector3 ray(Vector3 pos, Vector3 mov, int32_t n) {
    if (n < 0) return B; 
    float prevd = 0;
    face ff = get_dist(pos);
    while (true) {
        ff = get_dist(pos);
        if ((ff.d < prevd) && (ff.d < thr)) break;
        //if(ff.d == 0) break;
        prevd = ff.d;
        Vector3 diff = pos - cam;
        if (diff.length2() > RD2)  break;
        pos += mov * (ff.d * 0.99);
    }
    count++;
    Vector3 col = B;
    if (ff.d < thr) {

        Vector3 newmov = ff.n == 0 ? fl.reflect(pos, mov) : cb.reflect(pos, mov);
        Vector3 reflected = ray(Vector3(pos), newmov, n - 1);
        Vector3 u = light - pos;
        Vector3 v = Vector3(mov);
        u.normalize_ip();
        v.normalize_ip();
        col = lighting(pos, ff.n, reflected, det(u, ff.n == 0 ? fl.normal(pos) : cb.normal(pos)), det(u, v));
    }
    if (n == N) return postpro(pos, col);
    return col;
}
    
uint16_t w = 256;
uint16_t h = 128;
        
uint32_t m = w * h + w ;

void pixel(uint8_t x, uint8_t y) {
    
    float a1 = rot0 + tan((y - h/2) * _d);
    float a2 = rot1 + tan((x - w/2) * _d);
    Vector3 pos = cam;
    Vector3 mov = Vector3(cos(a1) * sin(a2), -sin(a1), -cos(a1) * cos(a2));
    Vector3 color = ray(pos, mov, N);
    uint16_t c = (int32_t)(color.z * 31) << 11 | (int32_t)(color.y * 63) << 5 | (int32_t)(color.x * 31);
    Color col(c);
    DrawPoint(x, y, col);
}

int RayMain(void){

  //AVR_Output_Pixel(RED, 64, 32);
  println("RayMain");
  int32_t a = 0;
  print("m = ");
    print(m);
    print("\n");
  for (uint32_t i = 0; i < m; i++)
  {


      a += 1;
      if (a >= m) a -= m;

      int32_t y = a / w;
      int32_t x = a % w;
      //AVR_Output_Pixel(GREEN, x, y);
      pixel(x, y);
      
  }
  //AVR_Output_Pixel(GREEN, 32, 32);
  
}

#endif // FIRST_RTX




#ifdef LEO_RTX

#define WIDTH 256
#define HEIGHT 128


void printTuple(char * name, float f1, float f2, float f3) {
    return;
    print(name);
    print(": ");
    print(f1*1000);
    print(" ");
    print(f2*1000);
    print(" ");
    print(f3*1000);
    print("\n");
}

class Pair {
public:
    Pair(int32_t first = 0, float second = 0) { this->first = first; this->second = second; }
    int32_t first;
    float second;
};

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

Vec3 BLACK = Vec3( 0, 0, 0 );
Vec3 Back = Vec3( 0.2, 0.2, 0.8 );
Vec3 LIGHT = Vec3( 1, 1, 1 );
float fov = M_PI / 2;
float m = tan(fov / 2);
float ratio = (float)WIDTH / HEIGHT;
float ambient = 0.1;
float FAR = 10000;
float thr = 0.001;

float det(Vec3 u, Vec3 v) {
    return u.x * v.x + u.y * v.y + u.z * v.z;
}

class Obj {
public:
    Obj(float alpha = 0, float metal = 0) {
        _metal = metal;
        _alpha = alpha;
        _type = 0;
    }

    Obj(int32_t type, Vec3 pos, Vec3 alb, float s, float alpha, float metal) {
        _metal = metal;
        _alpha = alpha;
        _type = type;
        _s = s; // size, or radius
        _pos = pos;
        _alb = alb;
    }

    Vec3 alb(Vec3 pos) {
        if (_type == 0) {
            float c = ((int32_t)(floor(pos.x / 4.0f)) & 1) ^ ((int32_t)(floor(pos.z / 4.0f)) & 1);
            return { c, 1.0f - c, 0 };
        }
        return _alb;
    }

    // used for cubes
    Pair getFace(Vec3 pos) {
        Vec3 diff = pos - _pos;
        float dx = fabs(diff.x), dy = fabs(diff.y), dz = fabs(diff.z);
        if (dx > dy) {
            if (dx > dz) {
                return { 0, dx };
            }
            return { 2, dz };
        }
        if (dy > dz) {
            return { 1, dy };
        }
        return { 2, dz };
    }

    float dist(Vec3 pos) {
        switch (_type) {
        case 0: return fabs(pos.y);
        case 1: return getFace(pos).second - _s;
        case 2: return (_pos - pos).len() - _s;
        default: return 0;
        }
    }

    Vec3 reflect(Vec3 pos, Vec3 mov) {
        switch (_type) {
        case 0: return Vec3(mov.x, -mov.y, mov.z);
        case 1: {
            int32_t i = getFace(pos).first;
            if (i == 0) return Vec3(-mov.x, mov.y, mov.z);
            if (i == 1) return Vec3(mov.x, -mov.y, mov.z);
            return Vec3(mov.x, mov.y, -mov.z);
        }
        case 2: {
            Vec3 diff = pos - _pos;
            diff.normalize();
            return diff * (det(mov, diff) * -2) + mov;
        }
        default: return BLACK;
        }
    }

    Vec3 normal(Vec3 pos) {
        switch (_type) {
        case 0: return Vec3(0, 1, 0);
        case 1: {
            Vec3 diff = pos - _pos;
            int32_t i = getFace(pos).first;
            if (i == 0) return Vec3(diff.x < 0 ? -1 : 1, 0, 0);
            if (i == 1) return Vec3(0, diff.y < 0 ? -1 : 1, 0);
            return Vec3(0, 0, diff.z < 0 ? -1 : 1);
        }
        case 2: {
            Vec3 diff = pos - _pos;
            diff.normalize();
            return diff;
        }
        default: return BLACK;
        }
    }

    int32_t _type; // 0: Floor, 1: Cube, 2: Ball
    Vec3 _pos;
    Vec3 _alb;
    float _s;
    float _alpha;
    float _metal;
};


class ObjPair {
public:
    ObjPair(float first = 0, Obj *second = NULL) { this->first = first; this->second = second; }
    float first;
    Obj *second;
};


// sphere
/* Vec3 light = {-2, 5, 10};
Vec3 cam = Vec3(0, 5, 20);
float rot[] = {0, 0}; */
// cube and sphere
Vec3 light = Vec3(7, 5, 10);
Vec3 cam = Vec3(25, 5, 15);
float rot[] = { 0, M_PI / 4 };
// infinite mirrors
/*
Vec3 light = Vec3(30, 5, 0);
Vec3 cam = Vec3(0, 5, 0);
float rot[] = {0, 0}; 
*/
/* Obj ball = Obj(2, Vec3(0, 5, 0), {1, 1, 1}, 5, 1, 1);
Obj flr = Obj(1, 1);
Obj* obj[] = {&ball, &flr}; */

Obj cube = Obj(1, Vec3(0, 5, 0), {1, 1, 1}, 4, 1, 0);
Obj ball = Obj(2, Vec3(8, 5, -7), {1, 0.9, 0}, 5, 1, 1);

Obj flr = Obj(1, 1);
Obj* obj[] = {&ball, &cube, &flr };
/*
Obj cube1 = Obj(1, Vec3(0, 5, -20), {1, 1, 1}, 5, 0.1, 1);
Obj cube2 = Obj(1, Vec3(0, 5, 10), {1, 1, 1}, 5, 0.1, 1);
Obj cube3 = Obj(1, Vec3(-10, 5, -10), {1, 1, 1}, 5, 0.1, 1);
Obj cube4 = Obj(1, Vec3(10, 5, -10), {1, 1, 1}, 5, 0.1, 1);
Obj cube5 = Obj(1, Vec3(-10, 5, 0), {1, 1, 1}, 5, 0.1, 1);
Obj cube6 = Obj(1, Vec3(10, 5, 0), {1, 1, 1}, 5, 0.1, 1);
Obj flr = Obj(1, 0.5);
Obj* obj[] = {&cube1, &cube2, &cube3, &cube4, &cube5, &cube6, &flr};
*/
int32_t N = 7;
int32_t s = 1;

float min2(float a, float b) {
    return a < b ? a : b;
}

Vec3 lighting(Vec3 pos, Obj* o, Vec3 col, float dif, float spec) {
    float t1 = o->_alpha;
    float t2 = o->_metal;
    Vec3 alb = o->alb(pos);
    if (dif < ambient) dif = ambient;
    if (spec < 0) spec = 0;
    return { min2(col.x * t2 + (alb.x * dif * t1 + spec * t2) * LIGHT.x, 1),
            min2(col.y * t2 + (alb.y * dif * t1 + spec * t2) * LIGHT.y, 1),
            min2(col.z * t2 + (alb.z * dif * t1 + spec * t2) * LIGHT.z, 1) };
}

Vec3 postPro(Vec3 pos, Vec3 col) {
    return col;
}

ObjPair getDist(Vec3 pos) {
    
    float d2 = -1;
    
    Obj* _o = nullptr;
    for(int32_t i = 0; i < sizeof(obj) / sizeof(obj[0]); i++) {
        Obj* o = obj[i];
        float d = o->dist(pos);
        if (d < 0) d = 0;
        if (d2 == -1 || d < d2) d2 = d, _o = o;
        
    }
    
    return ObjPair( d2, _o );
}

Vec3 ray(Vec3 pos, Vec3 mov, int32_t n) {
    float prevd = 0.000001; // small positive value to avoid division by zero
    if (n < 0) return BLACK;
    float dist = 0;
    Obj* o = nullptr;
    
    while (true) {
        
        ObjPair p = getDist(pos);
        
        dist = p.first, o = p.second;
        if (dist <= prevd && dist < thr) break;
        prevd = dist;

        if ((pos - cam).len2() > FAR) break;
        pos += mov * (dist * 0.99);
    }

    Vec3 col = Back;
    if (dist < thr) {
        Vec3 newmov = o->reflect(pos, mov);
        Vec3 reflected = ray(Vec3(pos), newmov, n - 1);
        Vec3 u = light - pos;
        Vec3 v = mov;
        u.normalize();
        v.normalize();
        col = lighting(pos, o, reflected, det(u, o->normal(pos)), det(u, v));
    }

    if (n == N) return postPro(pos, col);
    return col;
}


void draw_sqr(int32_t x, int32_t y, Vec3 col) {
        
    int32_t r = 31 * col.x, g = 63 * col.y, b = 31 * col.z;
    for (int32_t j = x; j < x + s && j < WIDTH; j++){
        for (int32_t i = y; i < y + s && i < HEIGHT; i++){
            uint16_t c = (b << 11) | (g << 5) | r;
            Color col = Color(c);
            DrawPoint((int)j, (int)i, col);
        }
    }
}

void pixel(float x, float y) {
    // fisheye, supports >180° fov
    // float a1 = rot[0] + tan((y - (HEIGHT>>1))*d), a2 = rot[1] + tan((x - (WIDTH>>1))*d);
    // Vec3 mov = {cos(a1)*sin(a2), -sin(a1), -cos(a1)*cos(a2)};

    float dx = m * ratio * (x / (float)WIDTH - 0.5f), dy = -m * (y / (float)HEIGHT - 0.5f), dz = -1;
    float cos0 = cos(rot[0]), sin0 = sin(rot[0]), cos1 = cos(rot[1]), sin1 = sin(rot[1]);
    // rotate ray according to camera
    Vec3 mov = Vec3(
        dx * cos1 + dy * sin0 * sin1 + dz * cos0 * cos1,
        dy * cos0 - dz * sin0,
        -dx * sin1 + dy * sin0 * cos1 + dz * cos0 * cos1
    );
    mov.normalize();
    Vec3 r = ray(cam, mov, N);
    draw_sqr(x, y, r);
    
}


void pixel_antialiasing(float x, float y, int factor = 2) {
    // fisheye, supports >180° fov
    // float a1 = rot[0] + tan((y - (HEIGHT>>1))*d), a2 = rot[1] + tan((x - (WIDTH>>1))*d);
    // Vec3 mov = {cos(a1)*sin(a2), -sin(a1), -cos(a1)*cos(a2)};
    Vec3 cols[16];
    for(int i = 0; i < factor * factor; i++)
    {
        cols[i] = Vec3(0, 0, 0);
        float dx = m * ratio * ((x + (i % factor) * (1.0f / (float)factor)) / (float)WIDTH - 0.5f);
        float dy = -m * ((y + (i / factor) * (1.0f / (float)factor)) / (float)HEIGHT - 0.5f);
        float dz = -1;
        float cos0 = cos(rot[0]), sin0 = sin(rot[0]), cos1 = cos(rot[1]), sin1 = sin(rot[1]);
        // rotate ray according to camera
        Vec3 mov = Vec3(
            dx * cos1 + dy * sin0 * sin1 + dz * cos0 * cos1,
            dy * cos0 - dz * sin0,
            -dx * sin1 + dy * sin0 * cos1 + dz * cos0 * cos1
        );
        mov.normalize();
        cols[i] = ray(cam, mov, N);
    }
    Vec3 r = Vec3(0, 0, 0);
    for(int i = 0; i < factor * factor; i++)
    {
        r = r + cols[i];
    }
    r.x = r.x / (factor * factor);
    r.y = r.y / (factor * factor);
    r.z = r.z / (factor * factor);
    draw_sqr(x, y, r); 
}

int RayMain(void){
    for (int32_t y = 0; y < HEIGHT; y += s) {
        for (int32_t x = 0; x < WIDTH; x += s) {
            pixel(x, y);
        }
    }
}

#endif // LEO_RTX