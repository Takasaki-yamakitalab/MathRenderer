project(MathRendererTest)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE MathRenderer)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_SOURCE_DIR}/dot_font_x11y23.bmp
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/dot_font_x11y23.bmp
)
