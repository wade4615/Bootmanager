
include_directories(${BOOTMANAGER_SOURCE_DIR}/hosttools/include/bootmanager/libs/fast486)

list(APPEND SOURCE
    debug.c
    fast486.c
    opcodes.c
    opgroups.c
    extraops.c
    common.c
    fpu.c)

add_library(fast486 ${SOURCE})
add_dependencies(fast486 xdk)
