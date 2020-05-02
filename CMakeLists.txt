cmake_minimum_required(VERSION 3.2.1)
cmake_policy(VERSION 3.2.1)

# Set the project name
project (BOOTMANAGER)

include(compilerflags.cmake)
include(gcc.cmake)
include(CMakeMacros.cmake)

enable_language(ASM)

include_directories(include)

 CreateBootSectorTarget(fat32 ${CMAKE_CURRENT_SOURCE_DIR}/fat32.S ${CMAKE_CURRENT_BINARY_DIR}/fat32.bin 7c00)
