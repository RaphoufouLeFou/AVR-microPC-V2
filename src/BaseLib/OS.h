#pragma once

#ifndef OS_H
#define OS_H

#include "Font.h"

#define NULL 0

enum Exception {
    EXCEPTION_DIVIDE_BY_ZERO,
    EXCEPTION_INVALID_OPCODE,
    EXCEPTION_STACK_FAULT,
    EXCEPTION_GENERAL_PROTECTION_FAULT,
    EXCEPTION_ARGUMENT_ERROR,
    EXCEPTION_SECURITY,
    EXCEPTION_OVERFLOW,
    EXCEPTION_ASSERTION,
    EXCEPTION_FILE,
};

void ThrowException(Exception e, int line, const char *file, const char* message, bool halt);
void PrintText(const char *text, uint8_t x, uint8_t y, Color color);
void DrawChar(char c, uint8_t x, uint8_t y, Color color);
void DrawCharOtherSens(char c, uint8_t x, uint8_t y, Color color);

#endif
