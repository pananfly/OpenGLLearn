cmake_minimum_required(VERSION 3.10) # 设置支持最低Cmake版本
set(NEED_COPY_LINK_FILE OFF)
# set(CMAKE_GNUtoMS ON) # 开启这个需要安装visual studio 才能把生成的dll.a转成.lib文件，但是我这里发现x86不能设置，每次都是默认x64，所以放弃这种方式。改为选择visual studio 的kit的方式进行编译，感觉没有好的方法了。

if (Apple)
  set (CMAKE_OSX_ARCHITECTURES i386)
  set (CUDA_64_BIT_DEVICE_CODE OFF)
endif (Apple)

if (CMAKE_HOST_WIN32)
    set(WINDOWS 1)
    # set( CMAKE_SUPPORT_WINDOWS_EXPORT_ALL_SYMBOLS 1 )
    # set(CMAKE_GNUtoMS:BOOL ON)
elseif(CMAKE_HOST_APPLE)
    set(MACOS 1)
elseif(CMAKE_HOST_UNIX)
    set(LINUX 1)
endif()
set(PROJECT_NAME_STR Test) # 设置项目名称为Test
project(${PROJECT_NAME_STR}) # set project name
message("-- CMAKE_BUILD_TYPE : ${CMAKE_BUILD_TYPE}") # 输出从cmake运行传过来的buildType参数是什么，如Debug和Release等
message("-- CAMKE_BUILD_GENERATOR : ${CAMKE_BUILD_GENERATOR}") #输出从cmake运行传过来的buildGenerator参数是什么，如x86和x64等，这个参数是自定义的，如果不传默认编译64位(我当前系统64位)
message("-- CMAKE_GENERATOR_TOOLSET : ${CMAKE_GENERATOR_TOOLSET}")
message("-- CMAKE_GENERATOR : ${CMAKE_GENERATOR}")

if (CMAKE_HOST_WIN32)
    if(${CMAKE_GENERATOR_TOOLSET} MATCHES "^.*x86.*$")
        set(CMAKE_VS_PLATFORM_NAME "x86")
    else()
        set(CMAKE_VS_PLATFORM_NAME "x64")
    endif()
    if(${CMAKE_GENERATOR} STREQUAL "MinGW Makefiles")
        set(NEED_COPY_LINK_FILE ON)
        if("${CAMKE_BUILD_GENERATOR}" MATCHES "^.*x86.*$") # 判断编译的版本
            set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32") # -m32 32bit 指定为编译32位
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32")
            set(GLFW_LINK_DIR ${PROJECT_SOURCE_DIR}/../libs/opengl/glfw/windows/x86/lib-static-ucrt) # 设置GLFW_LINK_DIR值为libs文件夹下windows平台对应的32位glfw依赖的库路径
        else()
            set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m64") # -m64 64bit
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64")
            set(GLFW_LINK_DIR ${PROJECT_SOURCE_DIR}/../libs/opengl/glfw/windows/x64/lib-static-ucrt)# 设置GLFW_LINK_DIR值为libs文件夹下windows平台对应的64位glfw依赖的库路径
        endif()
    elseif(${CMAKE_GENERATOR} MATCHES "^.*2012.*$") # Visual Studio 11 2012
        set(GLFW_LINK_DIR ${PROJECT_SOURCE_DIR}/../libs/opengl/glfw/windows/${CMAKE_VS_PLATFORM_NAME}/lib-vc2012)# 设置GLFW_LINK_DIR值为libs文件夹下windows平台对应的glfw依赖的库路径
    elseif(${CMAKE_GENERATOR} MATCHES "^.*2013.*$") # Visual Studio 12 2013
        set(GLFW_LINK_DIR ${PROJECT_SOURCE_DIR}/../libs/opengl/glfw/windows/${CMAKE_VS_PLATFORM_NAME}/lib-vc2013)# 设置GLFW_LINK_DIR值为libs文件夹下windows平台对应的glfw依赖的库路径
    elseif(${CMAKE_GENERATOR} MATCHES "^.*2015.*$") # Visual Studio 14 2015
        set(GLFW_LINK_DIR ${PROJECT_SOURCE_DIR}/../libs/opengl/glfw/windows/${CMAKE_VS_PLATFORM_NAME}/lib-vc2015)# 设置GLFW_LINK_DIR值为libs文件夹下windows平台对应的glfw依赖的库路径
    elseif(${CMAKE_GENERATOR} MATCHES "^.*2017.*$") # Visual Studio 15 2017
        set(GLFW_LINK_DIR ${PROJECT_SOURCE_DIR}/../libs/opengl/glfw/windows/${CMAKE_VS_PLATFORM_NAME}/lib-vc2017)# 设置GLFW_LINK_DIR值为libs文件夹下windows平台对应的glfw依赖的库路径
    elseif(${CMAKE_GENERATOR} MATCHES "^.*2019.*$") # Visual Studio 16 2019
        set(GLFW_LINK_DIR ${PROJECT_SOURCE_DIR}/../libs/opengl/glfw/windows/${CMAKE_VS_PLATFORM_NAME}/lib-vc2019)# 设置GLFW_LINK_DIR值为libs文件夹下windows平台对应的glfw依赖的库路径
    elseif(${CMAKE_GENERATOR} MATCHES "^.*2022.*$") # Visual Studio 17 2022
        set(GLFW_LINK_DIR ${PROJECT_SOURCE_DIR}/../libs/opengl/glfw/windows/${CMAKE_VS_PLATFORM_NAME}/lib-vc2022)# 设置GLFW_LINK_DIR值为libs文件夹下windows平台对应的glfw依赖的库路径
    endif()
elseif(CMAKE_HOST_APPLE)
    set(GLFW_LINK_DIR ${PROJECT_SOURCE_DIR}/../libs/opengl/glfw/osx/lib-x86_64)
elseif(CMAKE_HOST_UNIX)
endif()
message("-- GLFW_LINK_DIR : ${GLFW_LINK_DIR}")
link_directories(${GLFW_LINK_DIR}) # 指定glfw3的链接路径
add_executable(${PROJECT_NAME_STR} test.cpp ${PROJECT_SOURCE_DIR}/../libs/opengl/glad/src/glad.c) # set source files 添加编译源文件

# 指定依赖的头文件路径
include_directories(
${PROJECT_SOURCE_DIR}/../libs/opengl/glfw/include
${PROJECT_SOURCE_DIR}/../libs/opengl/glad/include
)

target_link_libraries(${PROJECT_NAME_STR} glfw3) # 链接程序和glfw库

# copy lib 如果是windows，则拷贝依赖的glfw3.dll库到编译出来的工程exe同路径下，免得运行的时候找不到依赖的库
if(CMAKE_HOST_WIN32 AND NEED_COPY_LINK_FILE)
    add_custom_command(TARGET ${PROJECT_NAME_STR}
        POST_BUILD
        COMMAND echo "copy dll files after make"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${GLFW_LINK_DIR}/glfw3.dll"
        $<TARGET_FILE_DIR:${PROJECT_NAME_STR}>)

endif()
#add_library(testlib SHARED test.cpp)
#add_library(testa STATIC test.cpp)

add_library(testdll SHARED testdll.cpp)
# if(CMAKE_HOST_WIN32)
#     add_custom_command(TARGET testdll
#         PRE_BUILD
#         COMMAND "C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\VC\\Auxiliary\\Build\\vcvarsall.bat" "x86")

# endif()
# add_library(teststatic STATIC testdll.cpp)