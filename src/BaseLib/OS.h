#ifndef OS_H
#define OS_H

#include <stdint.h>
#include "Font.h"
#include "baseLib.h"

void PrintText(const char *text, uint8_t x, uint8_t y, Color color);

void DrawChar(char c, uint8_t x, uint8_t y, Color color);

#endif
