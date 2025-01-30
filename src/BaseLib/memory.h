#pragma once
#ifndef MEMORY_H
#define MEMORY_H

#include "baseLib.h"
/*
void * malloc(uint16_t size);
void free(void * ptr);
*/
void memset(void * ptr, uint8_t value, uint16_t size);
void memcpy(void * dest, void * src, uint16_t size);

#endif // MEMORY_H