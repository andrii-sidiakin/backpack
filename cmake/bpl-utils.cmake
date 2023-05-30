
# adds an executable and creates a test to running it 
function(bpl_add_simple_test NAME)
    cmake_parse_arguments(ARG "" "GROUP_NAME" "SOURCES;LIBRARIES" ${ARGN})

    if(NOT ARG_GROUP_NAME)
        set(ARG_GROUP_NAME ${PROJECT_NAME})
    endif()

    set(TEST_TARGET test_${ARG_GROUP_NAME}_${NAME})
    set(TEST_NAME "${ARG_GROUP_NAME}:${NAME}")

    if(NOT ARG_SOURCES)
        set(ARG_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/${NAME}.cpp")
    endif()

    add_executable(${TEST_TARGET} ${ARG_SOURCES})

    if(ARG_LIBRARIES)
        target_link_libraries(${TEST_TARGET} PRIVATE ${ARG_LIBRARIES})
    endif()

    add_test(NAME ${TEST_NAME} COMMAND ${TEST_TARGET})

endfunction()

