@ECHO OFF

rmdir /s /q output-MinGW-i386
call configure
cd output-MinGW-i386
ninja
cd..