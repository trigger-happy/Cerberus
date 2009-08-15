cmake_minimum_required(VERSION 2.6)
project("Cerberus")

if(${WIN32})
	set(CMAKE_INCLUDE_PATH /usr/i686-pc-mingw32/sys-root/mingw/include/)
	set(QT_INCLUDE_DIR /usr/i686-pc-mingw32/sys-root/mingw/include)
	set(QT_LIBRARY_DIR /usr/i686-pc-mingw32/sys-root/mingw/lib)
else()
endif()

set(CMAKE_BUILD_TYPE "Release")

include_directories("${PROJECT_SOURCE_DIR}/include")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR})

add_subdirectory(src)
#add the ff into src/CMakeLists.txt
#install(TARGETS binary_file RUNTIME DESTINATION bin COMPONENT applications)
set(CPACK_PACKAGE_NAME "Cerberus")
set(CPACK_COMPONENTS_ALL applications)
set(CPACK_COMPONENT_APPLICATIONS_DISPLAY_NAME "Core Application")
set(CPACK_COMPONENT_APPLICATIONS_DESCRIPTION "Contains the core appliation")
#set(CPACK_COMPONENT_APPLICATION_DEPENDS libraries)
set(CPACK_COMPONENT_APPLICATIONS_GROUP "Runtime")
set(CPACK_COMPONENT_GROUP_APPLICATION_DESCRIPTION "The runtime stuff")
set(CPACK_PACKAGE_VENDOR "CompSAt")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "Cerberus")
include(CPack)
