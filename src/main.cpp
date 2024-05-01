#include "BaseLib/baseLib.h"
#include "BaseLib/OS.h"

int main(){
    Init();
    FillScreen(Color(0, 1, 0));
    PrintText("Hello World!", 10, 10, Color(1, 1, 1));
    
    return 0;
}

