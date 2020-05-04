
include_directories(${BOOTMANAGER_SOURCE_DIR}/hosttools/include/bootmanager/subsys)

list(APPEND SOURCE
    compses.c
    connect.c
    execpgm.c
    lookupss.c
    smclient.c
    precomp.h)

add_library(smlib ${SOURCE})
add_pch(smlib precomp.h SOURCE)
add_dependencies(smlib psdk)
