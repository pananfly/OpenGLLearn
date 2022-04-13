cmake_minimum_required(VERSION 3.10)
set(NEED_COPY_LINK_FILE OFF)
set(PROJECT_NAME_STR Triangle)

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

message("-- CMAKE_GENERATOR : ${CMAKE_GENERATOR}")
message("-- CMAKE_BUILD_TYPE : ${CMAKE_BUILD_TYPE}")
message("-- CAMKE_BUILD_GENERATOR : ${CAMKE_BUILD_GENERATOR}")
message("-- CMAKE_GENERATOR_TOOLSET  : ${CMAKE_GENERATOR_TOOLSET}")

project(${PROJECT_NAME_STR}) # set project name

if (CMAKE_HOST_WIN32)
    if(${CMAKE_GENERATOR_TOOLSET} MATCHES "^.*x86.*$")
        set(CMAKE_VS_PLATFORM_NAME "x86")
    else()
        set(CMAKE_VS_PLATFORM_NAME "x64")
    endif()
    if(${CMAKE_GENERATOR} STREQUAL "MinGW Makefiles" OR ${CMAKE_GENERATOR} STREQUAL "Unix Makefiles" OR ${CMAKE_GENERATOR} STREQUAL "Ninja")
        set(NEED_COPY_LINK_FILE ON)
        
        if("${CAMKE_BUILD_GENERATOR}" MATCHES "^.*x86.*$") # 判断编译的版本
            set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32") # -m32 32bit 指定为编译32位
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32") 
            set(CMAKE_VS_PLATFORM_NAME "x86")
        else()
            set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m64") # -m64 64bit
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64")
            set(CMAKE_VS_PLATFORM_NAME "x64")
        endif()
        set(GLFW_SUB_LIB_DIR "lib-static-ucrt")
    elseif(${CMAKE_GENERATOR} MATCHES "^.*2012.*$") # Visual Studio 11 2012
        set(GLFW_SUB_LIB_DIR "lib-vc2012")
    elseif(${CMAKE_GENERATOR} MATCHES "^.*2013.*$") # Visual Studio 12 2013
    set(GLFW_SUB_LIB_DIR "lib-vc2013")
    elseif(${CMAKE_GENERATOR} MATCHES "^.*2015.*$") # Visual Studio 14 2015
    set(GLFW_SUB_LIB_DIR "lib-vc2015")
    elseif(${CMAKE_GENERATOR} MATCHES "^.*2017.*$") # Visual Studio 15 2017
    set(GLFW_SUB_LIB_DIR "lib-vc2017")
    elseif(${CMAKE_GENERATOR} MATCHES "^.*2019.*$") # Visual Studio 16 2019
    set(GLFW_SUB_LIB_DIR "lib-vc2019")
    elseif(${CMAKE_GENERATOR} MATCHES "^.*2022.*$") # Visual Studio 17 2022
    set(GLFW_SUB_LIB_DIR "lib-vc2022")
    endif()
    set(GLFW_LINK_DIR ${PROJECT_SOURCE_DIR}/../libs/opengl/glfw/windows/${CMAKE_VS_PLATFORM_NAME}/${GLFW_SUB_LIB_DIR}) # 设置GLFW_LINK_DIR值为libs文件夹下windows平台对应的glfw依赖的库路径
elseif(CMAKE_HOST_APPLE)
    set(GLFW_LINK_DIR ${PROJECT_SOURCE_DIR}/../libs/opengl/glfw/osx/lib-x86_64)
elseif(CMAKE_HOST_UNIX)
endif()
message("-- GLFW_LINK_DIR : ${GLFW_LINK_DIR}")


include_directories(
${PROJECT_SOURCE_DIR}
${PROJECT_SOURCE_DIR}/../libs/opengl/glfw/include
${PROJECT_SOURCE_DIR}/../libs/opengl/glad/include
${PROJECT_SOURCE_DIR}/../libs/opengl/shader
${PROJECT_SOURCE_DIR}/../libs/opengl/stb
)
link_directories(${GLFW_LINK_DIR}) # 指定glfw3的链接路径

add_executable(${PROJECT_NAME_STR}
triangle.cpp
${PROJECT_SOURCE_DIR}/../libs/opengl/glad/src/glad.c 
${PROJECT_SOURCE_DIR}/../libs/opengl/shader/SimpleShader.cpp
${PROJECT_SOURCE_DIR}/../libs/opengl/shader/LocalShaderReader.cpp
${PROJECT_SOURCE_DIR}/../libs/opengl/stbimpl/stb_image.cpp
) # set source files
target_link_libraries(${PROJECT_NAME_STR} glfw3)

add_executable(texture
texture.cpp
${PROJECT_SOURCE_DIR}/../libs/opengl/glad/src/glad.c 
${PROJECT_SOURCE_DIR}/../libs/opengl/shader/SimpleShader.cpp
${PROJECT_SOURCE_DIR}/../libs/opengl/shader/LocalShaderReader.cpp
${PROJECT_SOURCE_DIR}/../libs/opengl/stbimpl/stb_image.cpp
) # set source files
target_link_libraries(texture glfw3)

add_executable(texture2
texture2.cpp
${PROJECT_SOURCE_DIR}/../libs/opengl/glad/src/glad.c 
${PROJECT_SOURCE_DIR}/../libs/opengl/shader/SimpleShader.cpp
${PROJECT_SOURCE_DIR}/../libs/opengl/shader/LocalShaderReader.cpp
${PROJECT_SOURCE_DIR}/../libs/opengl/stbimpl/stb_image.cpp
) # set source files
target_link_libraries(texture2 glfw3)

# copy lib
if(CMAKE_HOST_WIN32 AND NEED_COPY_LINK_FILE)
    add_custom_command(TARGET ${PROJECT_NAME_STR}
        POST_BUILD
        COMMAND echo "copy dll files after make"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${GLFW_LINK_DIR}/glfw3.dll"
        $<TARGET_FILE_DIR:${PROJECT_NAME_STR}>)

endif()
