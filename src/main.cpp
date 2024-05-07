#include "BaseLib/baseLib.h"
#include "BaseLib/OS.h"


int main(){
    Init();
    InitUART(9600);
    print("Hello World!\n");
    DrawPoint(10, 10, Color(1, 1, 1));

for(int k = 0; k < 1000; k++){
    for (int i = 0; i < 256; i++)
    {
        for (int j = 0; j < 128; j++)
        {
            DrawPoint(i, j, Color(1, 1, 1));
        }
    }
}

    while(1){
        char c = ReceiveUART();
        Delay10ms();
        printChar(c);
    }
    
    //DrawPoint(10, 10, Color(1, 1, 1));
    //WriteOutputs(0xFF);
    //DrawLine(0, 0, 255, 127, Color(1, 1, 1));
    //FillScreen(Color(0, 1, 0));
    //PrintText("Hello World!", 10, 10, Color(1, 1, 1));

    return 0;
}

