#include "BaseLib/baseLib.h"
#include "BaseLib/OS.h"

int main(){
    Init();
    FillScreen(Color(0, 0.5, 0));
    PrintText("Hello World!", 0, 0, Color(1, 1, 1));
    
    return 0;
}

