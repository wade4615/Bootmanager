
add_definitions(
    -D__NTDRIVER__
    -DKERNEL)

include_directories(
    ${BOOTMANAGER_SOURCE_DIR}/sdk/lib/drivers/sound/soundblaster
    ${BOOTMANAGER_SOURCE_DIR}/sdk/include/reactos/libs/sound)

add_library(audioleg devname.c hardware.c)
add_dependencies(audioleg bugcodes xdk)
