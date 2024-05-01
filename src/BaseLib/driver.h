extern "C" {

    #include "Fixed.h"
    /// @brief Initialize the IO pins 
    extern void AVR_Init(void);

    /// @brief Output a pixel to the screen at the given address with the given color
    /// @param Color 16-bit color value (5 bits blue, 6 bits green, 5 bits red)
    /// @param AddressX X coordinate of the pixel (0-255)
    /// @param AddressY Y coordinate of the pixel (0-127)
    extern void AVR_Output_Pixel(uint16_t Color, uint8_t AddressX, uint8_t AddressY);

    /// @brief Fill the screen with the given color
    /// @param Color 16-bit color value (5 bits blue, 6 bits green, 5 bits red)
    extern void AVR_Output_All(uint16_t Color);

    /// @brief Read the input pins and return the value
    /// @return input value from PORTC
    extern uint8_t AVR_Read_Inputs(void);
}