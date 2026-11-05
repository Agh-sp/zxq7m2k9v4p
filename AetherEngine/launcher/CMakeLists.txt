add_executable(aether_launcher src/main.cpp)

target_link_libraries(aether_launcher PRIVATE Aether::Engine)

set_target_properties(aether_launcher PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

add_custom_command(TARGET aether_launcher POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/assets
            $<TARGET_FILE_DIR:aether_launcher>/assets
    VERBATIM
)
