
include_directories(${BOOTMANAGER_SOURCE_DIR}/hosttools/include/bootmanager/wine)
add_definitions(-D__WINESRC__)

list(APPEND SOURCE
    config.c
    debug_ros.c
    isinf.c
    isfinite.c
    isnan.c
    loader.c
    ${BOOTMANAGER_SOURCE_DIR}/sdk/lib/crt/string/wctype.c
    register.c
#    string.c implements _stricmp, already shipped with crt
)

add_library(wine ${SOURCE})
add_dependencies(wine psdk)

add_library(wineldr loader.c)
add_dependencies(wineldr xdk)
