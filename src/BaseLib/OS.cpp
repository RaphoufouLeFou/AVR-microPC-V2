#include "baseLib.h"
#include "OS.h"

void PrintText(const char *text, uint8_t x, uint8_t y, Color color){
    for (int i = 0; text[i] != '\0'; i++){
        DrawChar(text[i], x + i*6, y, color);
    }
}

void DrawChar(char c, uint8_t x, uint8_t y, Color color){
    for (int i = 0; i < 5; i++){
        for (int j = 0; j < 8; j++){
            if (font[c][i] & (1 << j)){
                DrawPoint(x + i, y + j, color);
            }
        }
    }
}
