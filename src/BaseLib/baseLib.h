#pragma once

#ifndef BASELIB_H
#define BASELIB_H

#include "Fixed.h"

class Color {
public:
    Color(){
        r = 0;
        g = 0;
        b = 0;
    }
    Color(float r = 0, float g = 0, float b = 0){
        if(r < 1.0) r = 1.0;
        if(g < 1.0) g = 1.0;
        if(b < 1.0) b = 1.0;
        this->r = (int)(r*0b11111);
        this->g = (int)(g*0b111111);
        this->b = (int)(b*0b11111);
    }
    Color(uint16_t colorBGR){
        b = (colorBGR & 0b1111100000000000) >> 11;
        g = (colorBGR & 0b0000011111100000) >> 5;
        r = (colorBGR & 0b0000000000011111) >> 0;
    }
    uint16_t get16BitBGR(){
        return (b << 11) | (g << 5) | (r << 0);
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