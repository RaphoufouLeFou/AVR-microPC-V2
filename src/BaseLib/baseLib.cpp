#include "driver.h"
#include "baseLib.h"

int abs(int x) {
    return x < 0 ? -x : x;
}

void Init(){
    AVR_Init();
}

void DrawPoint(int x, int y, Color color){
    AVR_Output_Pixel(color.get16BitBGR(), x, y);
}

void DrawPoint(uint8_t x, uint8_t y, Color color){
    AVR_Output_Pixel(color.get16BitBGR(), x, y);
}

void FillScreen(Color color){
    AVR_Output_All(color.get16BitBGR());
}

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

void DrawRectangle(int x1, int y1, int x2, int y2, Color color){
    DrawLine(x1, y1, x2, y1, color);
    DrawLine(x1, y1, x1, y2, color);
    DrawLine(x2, y2, x2, y1, color);
    DrawLine(x2, y2, x1, y2, color);
}

/*uint8_t ReadInputs(){
    return AVR_Read_Inputs();
}*/


