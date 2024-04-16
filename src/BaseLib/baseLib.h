
#pragma once

#ifndef BASELIB_H
#define BASELIB_H

#include <stdint.h>

class Color {
public:
    Color(){
        r = 0;
        g = 0;
        b = 0;
    }
    /*Color(uint8_t r, uint8_t g, uint8_t b){
        this->r = r;
        this->g = g;
        this->b = b;
    }*/
    Color(float r, float g, float b){
        this->r = (int)(r*31);
        this->g = (int)(g*63);
        this->b = (int)(b*31);
    }
    Color(uint16_t colorBGR){
        b = (colorBGR & 0b1111100000000000) >> 11;
        g = (colorBGR & 0b0000011111100000) >> 5;
        r = (colorBGR & 0b0000000000011111);
    }
    uint16_t get16BitBGR(){
        return (b << 11) | (g << 5) | r;
    }
    uint8_t r;
    uint8_t g;
    uint8_t b;
};


void Init();
void DrawPoint(int x, int y, Color color);
void DrawPoint(uint8_t x, uint8_t y, Color color);
void FillScreen(Color color);
void DrawLine(int x1, int y1, int x2, int y2, Color color);
void DrawRectangle(int x1, int y1, int x2, int y2, Color color);
uint8_t ReadInputs();

#endif /* !BASELIB_H */