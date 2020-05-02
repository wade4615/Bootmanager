
cmake_minimum_required(VERSION 3.2.1)
cmake_policy(VERSION 3.2.1)

if(NOT CMAKE_VERSION MATCHES "ReactOS")
    message(WARNING "Building with \"${CMAKE_COMMAND}\", which is not the custom CMake included in RosBE, might cause build issues...")
endif()

if(POLICY CMP0058)
    # Ninja requires custom command byproducts to be explicit
    cmake_policy(SET CMP0058 OLD)
endif()

if(POLICY CMP0065)
    # Do not add flags to export symbols from executables without the ENABLE_EXPORTS target property
    cmake_policy(SET CMP0065 NEW)
endif()

project(REACTOS)

# Versioning
include(sdk/include/reactos/version.cmake)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INCLUDE_DIRECTORIES_PROJECT_BEFORE ON)
set(CMAKE_SHARED_LIBRARY_PREFIX "")
set(CMAKE_SHARED_MODULE_PREFIX "")
set(CMAKE_SKIP_PREPROCESSED_SOURCE_RULES TRUE)
set(CMAKE_SKIP_ASSEMBLY_SOURCE_RULES TRUE)
set(CMAKE_COLOR_MAKEFILE OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE OFF)
#set_property(GLOBAL PROPERTY RULE_MESSAGES OFF)

if(NOT ARCH)
    set(ARCH i386)
endif()
# Now the ARCH variable will be in lowercase.
# It is needed because STREQUAL comparison
# is case-sensitive.
# See http://cmake.3232098.n2.nabble.com/Case-insensitive-string-compare-td7580269.html
# for more information.
string(TOLOWER ${ARCH} ARCH)

# Compile options
if(ARCH STREQUAL "i386")
    include(sdk/cmake/config.cmake)
elseif(ARCH STREQUAL "amd64")
    include(sdk/cmake/config-amd64.cmake)
elseif(ARCH STREQUAL "arm")
    include(sdk/cmake/config-arm.cmake)
endif()

# Compiler flags handling
include(sdk/cmake/compilerflags.cmake)

add_definitions(-D__REACTOS__)

# Double escape, since CMake unescapes before putting it on the command-line, where it's unescaped again by GCC/CL.
add_definitions(-DREACTOS_SOURCE_DIR="${REACTOS_SOURCE_DIR}")
add_definitions(-DREACTOS_BINARY_DIR="${REACTOS_BINARY_DIR}")

# There doesn't seem to be a standard for __FILE__ being relative or absolute, so detect it at runtime.
file(RELATIVE_PATH _PATH_PREFIX ${REACTOS_BINARY_DIR} ${REACTOS_SOURCE_DIR})
add_compile_flags(-D__RELFILE__="&__FILE__[__FILE__[0] == '.' ? sizeof \\\"${_PATH_PREFIX}\\\" - 1 : sizeof REACTOS_SOURCE_DIR]")

if(MSVC_IDE)
    add_compile_flags("/MP")
endif()

# Bison and Flex support
# include(sdk/cmake/bison-flex.cmake)

if(NOT CMAKE_CROSSCOMPILING)
    set(TOOLS_FOLDER ${CMAKE_CURRENT_BINARY_DIR})
    add_definitions(-DTARGET_${ARCH})

    if(MSVC)
        if(ARCH STREQUAL "i386")
            add_definitions(/D_X86_ /D__i386__ /DWIN32 /D_WINDOWS)
        elseif(ARCH STREQUAL "amd64")
            add_definitions(-D_AMD64_ -D__x86_64__ /DWIN32 -D_WINDOWS)
        endif()
        if(MSVC_VERSION GREATER 1699)
            add_definitions(/D_ALLOW_KEYWORD_MACROS)
        endif()
        if(NOT USE_CLANG_CL)
            # FIXME: Inspect
            add_definitions(/Dinline=__inline)
        endif()
    endif()
    add_subdirectory(sdk/include/host)

    if(NOT MSVC)
        add_subdirectory(dll/win32/dbghelp)
    endif()
    add_subdirectory(sdk/tools)
    add_subdirectory(sdk/lib)

    set(NATIVE_TARGETS bin2c widl gendib cabman fatten hpp isohybrid mkhive mkisofs obj2bin spec2def geninc mkshelllink utf16le xml2sdb)
    if(NOT MSVC)
        list(APPEND NATIVE_TARGETS rsym pefixup)
    endif()

    export(TARGETS ${NATIVE_TARGETS} FILE ${CMAKE_BINARY_DIR}/ImportExecutables.cmake NAMESPACE native- )
    configure_file(sdk/cmake/host-tools.in ${CMAKE_BINARY_DIR}/TargetList.cmake)

else()
    # Add host tools target
    include(sdk/cmake/host-tools.cmake)
    setup_host_tools()
endif()
