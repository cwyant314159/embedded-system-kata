function(generate_artifacts exe_name)
    add_custom_command(TARGET ${exe_name} POST_BUILD
        COMMAND
            ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${exe_name}> ${exe_name}.hex
        COMMAND
            ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${exe_name}> ${exe_name}.bin
        COMMAND
            ${CMAKE_OBJDUMP} -rdCSw $<TARGET_FILE:${exe_name}> > ${exe_name}.lss
        COMMAND
            ${CMAKE_SIZE} -A -t $<TARGET_FILE:${exe_name}> > ${exe_name}.size.txt
    )
endfunction()