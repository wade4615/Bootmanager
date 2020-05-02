@ECHO OFF

rmdir /S /Q output-MinGW-i386
call configure
cd output-MinGW-i386
ninja -d explain
cd..

REM gcc -o fat32.o -c src/fat32.S