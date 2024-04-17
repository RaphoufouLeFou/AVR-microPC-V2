#include "driver.h"
#include "baseLib.h"

int abs(int x) {
    return x < 0 ? -x : x;
}

/// @brief Initialize the IO pins
void Init(){
    AVR_Init();
}

/// @brief Output a pixel to the screen at the given address with the given color
/// @param AddressX X coordinate of the pixel (0-255)
/// @param AddressY Y coordinate of the pixel (0-127)
/// @param Color 16-bit color value
void DrawPoint(int x, int y, Color color){
    AVR_Output_Pixel(color.get16BitBGR(), x, y);
}

/// @brief Output a pixel to the screen at the given address with the given color
/// @param AddressX X coordinate of the pixel (0-255)
/// @param AddressY Y coordinate of the pixel (0-127)
/// @param Color 16-bit color value 
void DrawPoint(uint8_t x, uint8_t y, Color color){
    AVR_Output_Pixel(color.get16BitBGR(), x, y);
}

/// @brief Fill the screen with the given color
/// @param Color 16-bit color value
void FillScreen(Color color){
    AVR_Output_All(color.get16BitBGR());
}

/// @brief Draw a line on the screen at the given address with the given color
/// @param x1 X coordinate of the start point (0-255)
/// @param y1 Y coordinate of the start point (0-127)
/// @param x2 X coordinate of the end point (0-255)
/// @param y2 Y coordinate of the end point (0-127)
/// @param color 16-bit color value
void DrawLine(int x1, int y1, int x2, int y2, Color color){
    int dx = abs (x2 - x1);
    int dy = abs (y2 - y1);
    int sx = x2 - x1 < 0 ? -1 : 1;
    int sy = y2 - y1 < 0 ? -1 : 1;
    int err = dx - dy;

    while (x1 != x2 || y1 != y2)
    {
        if (x1 < 256 && x1 >= 0 && y1 < 128 && y1 >= 0){
            AVR_Output_Pixel(color.get16BitBGR(), x1, y1);
            AVR_Output_Pixel(color.get16BitBGR(), x1, y1);
        }
        int e2 = err << 1;
        if (e2 > -dy) 
        {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx)
        {
            err += dx;
            y1 += sy;
        }
    }
}

/// @brief Draw the outline of a rectangle on the screen at the given address with the given color
/// @param x1 X coordinate of the start point (0-255)
/// @param y1 Y coordinate of the start point (0-127)
/// @param x2 X coordinate of the end point (0-255)
/// @param y2 Y coordinate of the end point (0-127)
/// @param color 16-bit color value
void DrawRectangle(int x1, int y1, int x2, int y2, Color color){
    DrawLine(x1, y1, x2, y1, color);
    DrawLine(x1, y1, x1, y2, color);
    DrawLine(x2, y2, x2, y1, color);
    DrawLine(x2, y2, x1, y2, color);
}

/// @brief Returns the state of the IO pins
/// @return the state of the IO pins
uint8_t ReadInputs(){
    return AVR_Read_Inputs();
}


