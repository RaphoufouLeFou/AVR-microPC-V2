#pragma once
#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H   

#include "baseLib.h"

struct File{
    const char * name;
    int16_t address;
    uint16_t size;
    File(const char * name, uint16_t size){
        this->name = name;
        this->size = size;
        this->address = -1;
    }
    File(){
        this->name = 0;
        this->size = 0;
        this->address = -1;
    }
};

enum FileError{
    FILE_OK = 0,
    FILE_ERROR_NONE,
    FILE_ERROR_NOT_FOUND,
    FILE_ERROR_INVALID,
    FILE_ERROR_OUT_OF_MEMORY,
};

FileError CreateFile(const char * name, uint16_t size, File * file);
FileError DeleteFile(File * file);
FileError ReadFile(File * file, uint16_t offset, uint16_t size, uint8_t * buffer);
FileError WriteFile(File * file, uint16_t offset, uint16_t size, uint8_t * buffer);
FileError RenameFile(File * file, const char * name);
FileError CopyFile(File * source, File * destination);

extern const char * FileErrorStrings[];

void assert(bool condition, const char * message);
void FileCheckError(FileError error);

#endif // FILE_SYSTEM_H