cmake_minimum_required(VERSION 3.10)

project(OPENGL_3D C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/glad.c")
file(GLOB HEADERS "include/*.h")
add_executable(${PROJECT_NAME}
    ${SOURCES} ${HEADERS}
)


set_property(TARGET ${PROJECT_NAME} PROPERTY AUTOMATIC_MOC ON)
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/glm
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)


add_library(stb_image STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/stb_image/stb_image.cpp
)
target_include_directories(stb_image PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/stb_image
)


add_library(glfw SHARED IMPORTED)
set_target_properties(glfw PROPERTIES
    IMPORTED_IMPLIB "${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw3dll.lib" 
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw3.dll"   
)


target_link_libraries(${PROJECT_NAME} glfw stb_image)

if(WIN32)
    target_link_libraries(${PROJECT_NAME} opengl32)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lstdc++fs")
endif()

add_custom_command(
    TARGET OPENGL_3D POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw3.dll"
        "$<TARGET_FILE_DIR:OPENGL_3D>"
    COMMENT "Copying glfw3.dll to output directory"
)
