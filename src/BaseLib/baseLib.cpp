#include "driver.h"
#include "baseLib.h"
#include "OS.h"
//#include "list.h"
#include "memory.h"

typedef struct {
    uint8_t x;
    uint8_t y;
    uint16_t color;
} Point;
/*
Queue<Point> points;

void AVR_INT7(){
    uint8_t val = 0;
    while(!val){
        if(points.length() == 0){
            asm("cli");
            return;
        }
        Point p = points.peek();
        val = AVR_Plot_Pixel(p.color, p.x, p.y);
        if(!val){
            points.pop();
        }
    }
    asm("cli");
}

ISR(INT7_vect){
    AVR_INT7();
}
*/
int abs(int x) {
    return x < 0 ? -x : x;
}

int max(int a, int b){
    return a > b ? a : b;
}

int min(int a, int b){
    return a < b ? a : b;
}

/// @brief Initialize the IO pins
void Init(){
    AVR_Init();
    DDRE &=~_BV(7); 		//INT0 en entrée 
	PORTE |= _BV(7); 		//Active la résistance de tirage sur l'entrée INT7
 
	EIMSK |= _BV(INT7);		//Interruption sur INT7
 
	EICRB |= 0b10000000;	//Interruption sur front  descendant donc dans le registre MCUCR ISC01 = 1 ISC00 =0
					        //Vu que MCUCR à comme valeur par defaut 0 on a juste à mettre ISC01 à 1 Cf : datasheet
 
 	//sei();				//Active les interruptions
 
}

void PlotPixel(int x, int y, Color color){
    //points.push(Point{x, y, color.get16BitBGR()});
    //points.push(Point{x, y, color.get16BitBGR()});
    print("Plotting pixel at x: ");
    print(x);
    print(" y: ");
    print(y);
    print("\n");
    //cli();
    //println(itoa(points.length()));
    //sei();
    AVR_Output_Pixel(color.get16BitBGR(), x, y);
}

/// @brief Output a pixel to the screen at the given address with the given color
/// @param AddressX X coordinate of the pixel (0-255)
/// @param AddressY Y coordinate of the pixel (0-127)
/// @param Color 16-bit color value
void DrawPoint(int x, int y, Color color){
    
    //AVR_Output_Pixel(color.get16BitBGR(), x*2, y);
    //AVR_Output_Pixel(color.get16BitBGR(), x*2, y);
    //AVR_Output_Pixel(color.get16BitBGR(), x*2+1, y);
    //AVR_Output_Pixel(color.get16BitBGR(), x*2+1, y);
    
    PlotPixel(x, y, color);
   // PlotPixel(x, y, color);
}




/// @brief Output a pixel to the screen at the given address with the given color
/// @param AddressX X coordinate of the pixel (0-255)
/// @param AddressY Y coordinate of the pixel (0-127)
/// @param Color 16-bit color value 
void DrawPoint(uint8_t x, uint8_t y, Color color){
    AVR_Output_Pixel(color.get16BitBGR(), x*2, y);
    AVR_Output_Pixel(color.get16BitBGR(), x*2, y);
    AVR_Output_Pixel(color.get16BitBGR(), x*2+1, y);
    AVR_Output_Pixel(color.get16BitBGR(), x*2+1, y);
}

/// @brief Fill the screen with the given color
/// @param Color 16-bit color value
void FillScreen(Color color){
    AVR_Output_All(color.get16BitBGR());
}

/// @brief Draw a line on the screen at the given address with the given color
/// @param x1 X coordinate of the start point (0-255)
/// @param y1 Y coordinate of the start point (0-127)
/// @param x2 X coordinate of the end point (0-255)
/// @param y2 Y coordinate of the end point (0-127)
/// @param color 16-bit color value
void DrawLine(int x1, int y1, int x2, int y2, Color color){
    int dx = abs (x2 - x1);
    int dy = abs (y2 - y1);
    int sx = x2 - x1 < 0 ? -1 : 1;
    int sy = y2 - y1 < 0 ? -1 : 1;
    int err = dx - dy;

    while (x1 != x2 || y1 != y2)
    {
        if (x1 < 256 && x1 >= 0 && y1 < 128 && y1 >= 0){
            AVR_Output_Pixel(color.get16BitBGR(), x1, y1);
            AVR_Output_Pixel(color.get16BitBGR(), x1, y1);
        }
        int e2 = err << 1;
        if (e2 > -dy) 
        {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx)
        {
            err += dx;
            y1 += sy;
        }
    }
}

/// @brief Draw the outline of a rectangle on the screen at the given address with the given color
/// @param x1 X coordinate of the start point (0-255)
/// @param y1 Y coordinate of the start point (0-127)
/// @param x2 X coordinate of the end point (0-255)
/// @param y2 Y coordinate of the end point (0-127)
/// @param color 16-bit color value
void DrawRectangle(int x1, int y1, int x2, int y2, Color color){
    DrawLine(x1, y1, x2, y1, color);
    DrawLine(x1, y1, x1, y2, color);
    DrawLine(x2, y2, x2, y1, color);
    DrawLine(x2, y2, x1, y2, color);
}

/// @brief Returns the state of the IO pins
/// @return the state of the IO pins
uint8_t ReadInputs(){
    return AVR_Read_Inputs();
}

/// @brief Write the output pins
/// @param value input value for PORTB
void WriteOutputs(uint8_t value){
    AVR_Write_Outputs(value);
}

/// @brief Initialize the UART at 9600 baud
void InitUART(int baudrate){
    uint16_t baud = (20000000/16/baudrate) - 1;
    AVR_Init_UART(baud);
    print("Initializing...\n");
}

/// @brief Send a byte over UART
/// @param data byte to send
void printChar(char character){
    AVR_Send_UART(character);
}

/// @brief Receive a byte over UART
/// @return received byte
unsigned char ReceiveUART(){
    return AVR_Receive_UART();
}

const char * ReceiveUARTString(){
    unsigned char buffer[256];
    AVR_Receive_UART_String(buffer);
    return (const char *)buffer;
}

/// @brief Delay for 1 second
void Delay(){
    AVR_Delay();
}

/// @brief Delay for 10ms
void Delay10ms(){
    AVR_Delay_10ms();
}

/// @brief Print a string to the UART
/// @param str the string to print
void print(const char* str){
    while(*str){
        AVR_Send_UART(*str);
        str++;
    }
}

void println(const char* str){
    print(str);
    printChar('\n');
}

void print(int i){
    print(itoa(i));
}

uint16_t DEBUG(){
    return AVR_DEBUG();
}

const char * itoa(int i){
    static char buffer[14];
    int j = 0;
    if(i < 0){
        buffer[j] = '-';
        j++;
        i = -i;
    }
    if(i == 0){
        buffer[j] = '0';
        j++;
    }
    while(i){
        buffer[j] = i % 10 + '0';
        i /= 10;
        j++;
    }
    buffer[j] = 0;
    for(int k = 0; k < j/2; k++){
        char temp = buffer[k];
        buffer[k] = buffer[j - k - 1];
        buffer[j - k - 1] = temp;
    }
    return buffer;
}

uint8_t ReadRom(uint16_t address){
    return AVR_Read_Rom(address);
}

void WriteRom(uint16_t address, uint8_t data){
    AVR_Write_Rom(address, data);
}

bool strcmp(const char* a, const char* b){
    while(*a && *b){
        if(*a != *b){
            return false;
        }
        a++;
        b++;
    }
    return *a == *b;
}
