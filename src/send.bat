avr-g++ -c -O3 -w -mmcu=ATmega128a -DF_CPU=20000000L main.cpp -o out.o
avr-gcc -O3 -Wl,--gc-sections -mmcu=atmega128a BaseLib/driver.o out.o BaseLib/lib.o BaseLib/OS.o -o image.elf
avr-objcopy -Oihex -R.eeprom image.elf image.hex -v
avr-size -A image.elf