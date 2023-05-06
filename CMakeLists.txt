cmake_minimum_required(VERSION 3.5)

project(hello_library)

############################################################
# Create a library
############################################################

#Generate the shared library from the library sources
add_library(hello_library SHARED 
    src/pobject.cpp
	src/pobject.h
	src/plugin.cpp
	src/plugin.h
    src/imodule.cpp
    src/imodule.h
    src/iplugin.cpp
    src/iplugin.h
    src/ipluginmanager.cpp
    src/ipluginmanager.h
    src/pluginmanager.cpp
    src/pluginmanager.h
    src/dynlib.cpp
    src/dynlib.h
    src/platform.cpp
    src/platform.h
)

add_library(hello::library ALIAS hello_library)

target_include_directories(hello_library
    PUBLIC 
        ${PROJECT_SOURCE_DIR}/include
)

set_property(TARGET hello_library PROPERTY FOLDER "plugin")

#Tutorial-1
add_library(Tutorial1_library SHARED
    src/tutorial1/Tutorial1.cpp
    src/tutorial1/Tutorial1.h
    src/tutorial1/HelloWorld1.cpp
    src/tutorial1/HelloWorld1.h
)

add_library(Tutorial1::library ALIAS Tutorial1_library)

target_include_directories(Tutorial1_library
    PUBLIC 
        ${PROJECT_SOURCE_DIR}/include
)

set_property(TARGET Tutorial1_library PROPERTY FOLDER "plugin")

############################################################
# Create an executable
############################################################

# Add an executable with the above sources
add_executable(hello_binary
    src/main.cpp
    src/ipluginmanager.cpp
    src/ipluginmanager.h
    src/pluginmanager.cpp
    src/pluginmanager.h
    src/dynlib.cpp
    src/dynlib.h
)

# link the new testPlugin target with the hello_binary target
target_link_libraries( hello_binary
    PRIVATE 
        hello::library
)

set_property(TARGET hello_binary PROPERTY FOLDER "plugin")

# 作为插件使用导出 必须首先声明dll 导出模式 具体实现参考宏定义逻辑
add_definitions(-DPLUGIN_EXPORTS)
add_definitions(-DDYNAMIC_PLUGIN)