#include "BaseLib/baseLib.h"
#include "BaseLib/OS.h"
#include "BaseLib/FileSystem.h"
#include "BaseLib/memory.h"
#include "BaseLib/list.h"

int main(){
    Init();
    InitUART(9600);

    File f;
    CreateFile("test", 12, &f);
    //WriteFile(&f, 0, 12, (uint8_t *) "Hello World");
    uint8_t * buffer = static_cast<uint8_t*>(malloc(12));
    ReadFile(&f, 0, 12, buffer);
    println((const char*)buffer);

    uint8_t * mem = static_cast<uint8_t*>(malloc(12));
    memcpy(mem, (void *)"lololololol", 12);
    print((const char*)mem);

    print("\n\n");

    uint8_t * mem2 = static_cast<uint8_t*>(malloc(15));
    memcpy(mem2, (void *)"Test de malloc", 15);
    print((const char*)mem2);

    print("\n\n");

    free(mem);
    free(mem2);

    print("\n\n");

    print("Testing list\n");

    List<int> list;
    list.push_back(1);
    list.push_back(2);

    *list[0] = 3;

    print(itoa(*list[0]));
    print(itoa(*list[1]));

    print("\n");

    print(itoa(list.length()));

    print("\n");

    print(itoa(list.pop_back()));
    print(itoa(list.pop_back()));

    print("\n");

    print(list.length());


    //WriteOutputs(0xFF);
    //DEBUG();

    for(int i = 0; i < 256; i++){
        for (int j = 0; j < 128; j++){
            DrawPoint(i, j, Color(0, 0, 1));
        }
    }


    PrintText("Quoi?", 10, 10, Color(1, 1, 1));
    PrintText("Feur", 10, 20, Color(1, 1, 1));

    print("Testing OS\n");
    while(1){
        //DrawPoint(10, 10, Color(1, 1, 1));
        //FillScreen(Color(0, 1, 0));
    }
    
    //DrawPoint(10, 10, Color(1, 1, 1));

    //DrawLine(0, 0, 255, 127, Color(1, 1, 1));
    //FillScreen(Color(0, 1, 0));
    //PrintText("Hello World!", 10, 10, Color(1, 1, 1));

    return 0;
}

