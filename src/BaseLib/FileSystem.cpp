#include "FileSystem.h"
#include "OS.h"

//uint16_t Adresses[256];
//uint16_t Size[256];
File* Files[1];

uint8_t FilesCount = 0;


const char * FileErrorStrings[] = {
    "No error",
    "Unknown error",
    "File not found",
    "Invalid file",
    "Out of memory",
};

FileError CreateFile(const char * name, uint16_t size, File * file){
    if(size == 0){
        return FILE_ERROR_INVALID;
    }
    if(*name == '\0' || name == 0) {
        return FILE_ERROR_INVALID;
    }
    file->name = name;
    file->size = size;
    
    //DEBUG
    file->address = 0;
    if(FilesCount != 0) return FILE_ERROR_OUT_OF_MEMORY;
    //!DEBUG

    if(FilesCount >= 256){
        delete file;
        return FILE_ERROR_OUT_OF_MEMORY;
    }
    for(int i = 0; i < FilesCount; i++){
        if(strcmp(Files[i]->name, name) == 0){
            return FILE_ERROR_INVALID;
        }
    }
    Files[FilesCount] = file;
    FilesCount++;
    return FILE_OK;
}

FileError DeleteFile(File * file){
    for(int i = 0; i < FilesCount; i++){
        if(Files[i] == file){
            for(int j = i; j < FilesCount - 1; j++){
                Files[j] = Files[j + 1];
            }
            FilesCount--;
            return FILE_OK;
        }
    }
    return FILE_ERROR_NOT_FOUND;
}

FileError ReadFile(File * file, uint16_t offset, uint16_t size, uint8_t * buffer){
    for(int i = 0; i < FilesCount; i++){
        if(Files[i] == file){
            if(offset + size > file->size){
                return FILE_ERROR_INVALID;
            }
            if(file->address == -1){
                return FILE_ERROR_INVALID;
            }
            for(int j = offset; j < offset + size; j++){
                buffer[j] = ReadRom(file->address + j);
            }
            return FILE_OK;
        }
    }
    return FILE_ERROR_NOT_FOUND;
}

FileError WriteFile(File * file, uint16_t offset, uint16_t size, uint8_t * buffer){
    for(int i = 0; i < FilesCount; i++){
        if(Files[i] == file){
            if(offset + size > file->size){
                return FILE_ERROR_INVALID;
            }
            if(file->address == -1){
                return FILE_ERROR_INVALID;
            }
            for(int j = offset; j < offset + size; j++){
                WriteRom(file->address + j, buffer[j]);
            }
            return FILE_OK;
        }
    }
    return FILE_ERROR_NOT_FOUND;
}

FileError RenameFile(File * file, const char * name){
    if(*name == '\0' || name == 0) {
        return FILE_ERROR_INVALID;
    }
    for(int i = 0; i < FilesCount; i++){
        if(Files[i] == file){
            for(int j = 0; j < FilesCount; j++){
                if(strcmp(Files[j]->name, name) == 0){
                    return FILE_ERROR_INVALID;
                }
            }
            file->name = name;
            return FILE_OK;
        }
    }
    return FILE_ERROR_NOT_FOUND;
}

FileError CopyFile(File * source, File * destination){
    // TODO
    return FILE_ERROR_NONE;
}

void assert(bool condition, const char * message){
    if(!condition){
        ThrowException(EXCEPTION_ASSERTION, __LINE__, __FILE__, message, 1);
    }
}

void FileCheckError(FileError error){
    if(error != FILE_OK){
        ThrowException(EXCEPTION_FILE, __LINE__, __FILE__, FileErrorStrings[error], 1);
    }
}