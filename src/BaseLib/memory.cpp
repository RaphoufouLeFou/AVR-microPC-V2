#include "memory.h"
#include "OS.h"

#define MEMORY_SIZE 4096

#include <avr/io.h>

struct __freelist {
	uint16_t sz;
	struct __freelist *nx;
};

extern char __heap_start;
extern char __heap_end;
#define STACK_POINTER() ((char *)AVR_STACK_POINTER_REG)

uint16_t __malloc_margin = 32;
char *__malloc_heap_start = &__heap_start;
char *__malloc_heap_end = &__heap_end;

char *__brkval;
struct __freelist *__flp;
/*
void * malloc(uint16_t len)
{
	struct __freelist *fp1, *fp2, *sfp1, *sfp2;
	char *cp;
	uint16_t s, avail;
	
	if (len < sizeof(struct __freelist) - sizeof(uint16_t))
		len = sizeof(struct __freelist) - sizeof(uint16_t);

	for (s = 0, fp1 = __flp, fp2 = 0;
	     fp1;
	     fp2 = fp1, fp1 = fp1->nx) {
		if (fp1->sz < len)
			continue;
		if (fp1->sz == len) {

			if (fp2)
				fp2->nx = fp1->nx;
			else
				__flp = fp1->nx;
			return &(fp1->nx);
		}
		else {
			if (s == 0 || fp1->sz < s) {
				
				s = fp1->sz;
				sfp1 = fp1;
				sfp2 = fp2;
			}
		}
	}

	if (s) {
		if (s - len < sizeof(struct __freelist)) {
			
			if (sfp2)
				sfp2->nx = sfp1->nx;
			else
				__flp = sfp1->nx;
			return &(sfp1->nx);
		}

		cp = (char *)sfp1;
		s -= len;
		cp += s;
		sfp2 = (struct __freelist *)cp;
		sfp2->sz = len;
		sfp1->sz = s - sizeof(uint16_t);
		return &(sfp2->nx);
	}

	if (__brkval == 0)
		__brkval = __malloc_heap_start;
	cp = __malloc_heap_end;
	if (cp == 0)
		cp = STACK_POINTER() - __malloc_margin;
	if (cp <= __brkval)

	  return 0;
	avail = cp - __brkval;

	if (avail >= len && avail >= len + sizeof(uint16_t)) {
		fp1 = (struct __freelist *)__brkval;
		__brkval += len + sizeof(uint16_t);
		fp1->sz = len;
		return &(fp1->nx);
	}
	return 0;
}

void free(void *p)
{
	struct __freelist *fp1, *fp2, *fpnew;
	char *cp1, *cp2, *cpnew;

	if (p == 0)
		return;

	cpnew = (char*)p;
	cpnew -= sizeof(uint16_t);
	fpnew = (struct __freelist *)cpnew;
	fpnew->nx = 0;

	if (__flp == 0) {
		if ((char *)p + fpnew->sz == __brkval)
			__brkval = cpnew;
		else
			__flp = fpnew;
		return;
	}

	for (fp1 = __flp, fp2 = 0;
	     fp1;
	     fp2 = fp1, fp1 = fp1->nx) {
		if (fp1 < fpnew)
			continue;
		cp1 = (char *)fp1;
		fpnew->nx = fp1;
		if ((char *)&(fpnew->nx) + fpnew->sz == cp1) {
			fpnew->sz += fp1->sz + sizeof(uint16_t);
			fpnew->nx = fp1->nx;
		}
		if (fp2 == 0) {
			__flp = fpnew;
			return;
		}
		break;
	}
	fp2->nx = fpnew;
	cp2 = (char *)&(fp2->nx);
	if (cp2 + fp2->sz == cpnew) {
		fp2->sz += fpnew->sz + sizeof(uint16_t);
		fp2->nx = fpnew->nx;
	}
	for (fp1 = __flp, fp2 = 0;
	     fp1->nx != 0;
	     fp2 = fp1, fp1 = fp1->nx);
	cp2 = (char *)&(fp1->nx);
	if (cp2 + fp1->sz == __brkval) {
		if (fp2 == NULL)
			__flp = NULL;
		else
			fp2->nx = NULL;
		__brkval = cp2 - sizeof(uint16_t);
	}
}
*/
void memset(void * ptr, uint8_t value, uint16_t size){
    if(!ptr) ThrowException(EXCEPTION_ARGUMENT_ERROR, __LINE__, __FILE__, "ptr is null", 0);
    uint8_t * p = (uint8_t *)ptr;
    for(int i = 0; i < size; i++){
        *p++ = value;
    }
}

void memcpy(void * dest, void * src, uint16_t size){
    if(!dest){
        ThrowException(EXCEPTION_ARGUMENT_ERROR, __LINE__, __FILE__, "dest is null", 0);
        return;
    } 
    if(!src){
        ThrowException(EXCEPTION_ARGUMENT_ERROR, __LINE__, __FILE__, "src is null", 0);
        return;
    } 
    uint8_t * d = (uint8_t *)dest;
    uint8_t * s = (uint8_t *)src;
    for(int i = 0; i < size; i++){
        *d++ = *s++;
    }
}