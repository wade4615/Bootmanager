
add_definitions(
    -D__NTDRIVER__
    -DKERNEL)

include_directories(BEFORE ${BOOTMANAGER_SOURCE_DIR}/hosttools/include/bootmanager/libs/sound)

list(APPEND SOURCE
    dsp_io.c
    version.c
    speaker.c
    rate.c
    mixer.c)

add_library(soundblaster ${SOURCE})
add_pch(soundblaster precomp.h)
add_dependencies(soundblaster bugcodes xdk)
