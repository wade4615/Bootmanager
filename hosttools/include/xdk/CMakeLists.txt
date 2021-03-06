
file(GLOB xdk_headers "*.h")

function(add_xdk_header _template _out)
    get_filename_component(_out_filename ${_out} NAME)
    set(_temp_file ${CMAKE_CURRENT_BINARY_DIR}/${_out_filename})
    add_custom_command(
            OUTPUT ${BOOTMANAGER_BINARY_DIR}/${_out}
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${_temp_file} ${BOOTMANAGER_BINARY_DIR}/${_out}
            DEPENDS ${_temp_file})

    add_custom_command(
            OUTPUT ${_temp_file}
            COMMAND native-hpp ${_template} ${_temp_file}
            DEPENDS ${_template} ${xdk_headers} native-hpp
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    set(xdk_files "${xdk_files};${BOOTMANAGER_BINARY_DIR}/${_out}" PARENT_SCOPE)
endfunction()

add_xdk_header(wdm.template.h hosttools/include/ddk/wdm.h)
add_xdk_header(ntddk.template.h hosttools/include/ddk/ntddk.h)
add_xdk_header(ntifs.template.h hosttools/include/ddk/ntifs.h)
add_xdk_header(devioctl.template.h hosttools/include/psdk/devioctl.h)
add_xdk_header(ntdef.template.h hosttools/include/psdk/ntdef.h)
add_xdk_header(winnt.template.h hosttools/include/psdk/winnt.h)
add_custom_target(xdk DEPENDS ${xdk_files})
