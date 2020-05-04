
add_definitions(
    -D__NTDRIVER__
    -DKERNEL)

include_directories(
    ${BOOTMANAGER_SOURCE_DIR}/sdk/lib/drivers/sound/soundblaster
    ${BOOTMANAGER_SOURCE_DIR}/hosttools/include/reactos/libs/sound)

add_library(audio time.c)
add_dependencies(audio bugcodes xdk)
