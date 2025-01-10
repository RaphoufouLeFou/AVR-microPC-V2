avr-g++ -c -O3 -w -mmcu=ATmega128a -DF_CPU=20000000L main.cpp -o out.o
avr-g++ -c -O3 -w -mmcu=ATmega128a -DF_CPU=20000000L BaseLib/OS.cpp -o BaseLib/OS.o
avr-g++ -c -O3 -w -mmcu=ATmega128a -DF_CPU=20000000L BaseLib/baselib.cpp -o BaseLib/lib.o
avr-g++ -c -O3 -w -mmcu=ATmega128a -DF_CPU=20000000L BaseLib/driver.S -o BaseLib/driver.o
avr-g++ -c -O3 -w -mmcu=ATmega128a -DF_CPU=20000000L BaseLib/FileSystem.cpp -o BaseLib/FileSystem.o
avr-g++ -c -O3 -w -mmcu=ATmega128a -DF_CPU=20000000L BaseLib/memory.cpp -o BaseLib/memory.o
avr-g++ -c -O3 -w -mmcu=ATmega128a -DF_CPU=20000000L BaseLib/list.cpp -o BaseLib/list.o
avr-gcc -O3 -Wl,--gc-sections,--section-start=.FAR_MEM1=0x10000,--section-start=.FAR_MEM2=0x20000,--section-start=.FAR_MEM3=0x30000 -mmcu=atmega128a -w -flto -g BaseLib/driver.o out.o BaseLib/lib.o BaseLib/OS.o BaseLib/FileSystem.o BaseLib/memory.o -o image.elf
avr-objcopy -Oihex -R.eeprom image.elf image.hex -v
avr-size -C -x image.elf
BaseLib/Run.bat