add_executable(aether_sandbox src/main.cpp)

target_link_libraries(aether_sandbox PRIVATE Aether::Engine)

set_target_properties(aether_sandbox PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

# Copy assets next to executable (best effort)
add_custom_command(TARGET aether_sandbox POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/assets
            $<TARGET_FILE_DIR:aether_sandbox>/assets
    VERBATIM
)
