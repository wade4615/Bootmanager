
include_directories(${BOOTMANAGER_SOURCE_DIR}/hosttools/include/reactos/subsys)

add_library(lsalib lsa.c)
add_dependencies(lsalib xdk)

add_library(ntlsalib lsa.c)
target_compile_definitions(ntlsalib PRIVATE _NTSYSTEM_)
add_dependencies(ntlsalib xdk)
