cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME Triangle)

if (Apple)
  set (CMAKE_OSX_ARCHITECTURES i386)
  set (CUDA_64_BIT_DEVICE_CODE OFF)
endif (Apple)

if (CMAKE_HOST_WIN32)
    set(WINDOWS 1)
elseif(CMAKE_HOST_APPLE)
    set(MACOS 1)
elseif(CMAKE_HOST_UNIX)
    set(LINUX 1)
endif()

message("-- CMAKE_C_FLAGS: ${CMAKE_C_FLAGS}")
message("-- CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")

#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32") # -m64 64bit, -m32 32bit
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32")
project(${PROJECT_NAME}) # set project name
if("${CMAKE_C_FLAGS}" MATCHES "^.*-m32.*$")
    if (CMAKE_HOST_WIN32)
        set(GLFW_LINK_DIR ${PROJECT_SOURCE_DIR}/../libs/opengl/glfw/windows/static-ucrt)
    elseif(CMAKE_HOST_APPLE)
        set(GLFW_LINK_DIR ${PROJECT_SOURCE_DIR}/../libs/opengl/glfw/osx/lib-x86_64)
    elseif(CMAKE_HOST_UNIX)
    endif()
else() # default to link 64 bit lib
    if (CMAKE_HOST_WIN32)
        set(GLFW_LINK_DIR ${PROJECT_SOURCE_DIR}/../libs/opengl/glfw/windows/static-ucrt-w64)
    elseif(CMAKE_HOST_APPLE)
        set(GLFW_LINK_DIR ${PROJECT_SOURCE_DIR}/../libs/opengl/glfw/osx/lib-x86_64)
    elseif(CMAKE_HOST_UNIX)
    endif()
endif()
link_directories(${GLFW_LINK_DIR})
add_executable(${PROJECT_NAME} triangle.cpp ${PROJECT_SOURCE_DIR}/../libs/opengl/glad/src/glad.c) # set source files

include_directories(
${PROJECT_SOURCE_DIR}/../libs/opengl/glfw/include
${PROJECT_SOURCE_DIR}/../libs/opengl/glad/include
)

target_link_libraries(${PROJECT_NAME} glfw3)

# copy lib
if(CMAKE_HOST_WIN32)
    add_custom_command(TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND echo "copy dll files after make"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${GLFW_LINK_DIR}/glfw3.dll"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)

endif()
#add_library(testlib SHARED test.cpp)
#add_library(testa STATIC test.cpp)
