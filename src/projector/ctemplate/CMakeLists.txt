#Copyright (C) 2009 Wilhansen Li

#This program is free software; you can redistribute it and/or
#modify it under the terms of the GNU General Public License
#as published by the Free Software Foundation; either version 2
#of the License, or (at your option) any later version.

#This program is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#GNU General Public License for more details.

#You should have received a copy of the GNU General Public License
#along with this program; if not, write to the Free Software
#Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

cmake_minimum_required(VERSION 2.6)

project("ctemplate")
set(PACKAGE_NAME "\"ctemplate\"")
set(PACKAGE_VERSION "\"0.96\"")
set(PACKAGE_STRING "${PACKAGE_NAME}\" \"${PACKAGE_VERSION}")
set(PACKAGE_BUGREPORT "\"opensource@google.com\"")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules")

option(BUILD_SHARED_LIBS "Build ctemplate as a shared library" false)

include(CheckFunctionExists)
include(CheckTypeSize)
include(CheckIncludeFile)
include(CheckLibraryExists)
include(CheckCXXSourceCompiles)
include(CheckCXXHash)
include(FindThreads)
include(CheckDirent)
include(CheckCompilerCharacteristics)
include(CheckHaveAttribute)

check_function_exists(getopt_long HAVE_GETOPT_LONG)

check_cxx_source_compiles("
	namespace Outer {
		namespace Inner { int i = 0; }
	} 
	int main() {
		using namespace Outer::Inner;
		return i;
	}"
HAVE_NAMESPACES)

if ( HAVE_NAMESPACES )
	#I think this is pretty standard
	set(STL_NAMESPACE "std")
endif()

macro(NORMALIZE_BOOL B)
	if ( ${B} )
		set(${B} 1)
	endif()
endmacro()
# These are the types I need.  We look for them in either stdint.h,
# sys/types.h, or inttypes.h, all of which are part of the default-includes.
check_type_size(uint32_t UINT32_T)
normalize_bool(HAVE_UINT32_T)
check_type_size(u_int32_t U_INT32_T)
normalize_bool(HAVE_U_INT32_T)
check_type_size(__int32 __INT32)
normalize_bool(HAVE___INT32)
check_type_size(uint64_t UINT64_T)
normalize_bool(HAVE_UINT64_T)
check_type_size(u_int64_t U_INT64_T)
normalize_bool(HAVE_U_INT64_T)
check_type_size(__int64 __INT64)
normalize_bool(HAVE___INT64)

check_include_file(inttypes.h HAVE_INTTYPES_H)
check_include_file(dlfcn.h HAVE_DLFCN_H)
check_dirent()
check_include_file(sys/stat.h HAVE_SYS_STAT_H)
check_include_file(sys/types.h HAVE_SYS_TYPES_H)

check_include_file(unistd.h HAVE_UNISTD_H)
check_include_file(memory.h HAVE_MEMORY_H)
check_include_file(stdlib.h HAVE_STDLIB_H)
check_include_file(strings.h HAVE_STRINGS_H)
check_include_file(string.h HAVE_STRING_H)

check_cxx_hash()
check_compiler_characteristics()
check_have_attribute()

check_include_file(byteswap.h HAVE_BYTESWAP_H)                         # Linux (GNU in general)
check_include_file(libkern/OSByteOrder.h HAVE_LIBKERN_OSBYTEORDER_H)   # OS X
check_include_file(sys/byteorder.h HAVE_SYS_BYTEORDER_H)               # Solaris 10
check_include_file(endian.h HAVE_ENDIAN_H)                             # Linux
check_include_file(machine/endian.h HAVE_MACHINE_ENDIAN_H)             # OS X
check_include_file(sys/endian.h HAVE_SYS_ENDIAN_H)                     # FreeBSD
check_include_file(sys/isa_defs.h HAVE_SYS_ISA_DEFS_H)                 # Solaris 10

if ( NOT DEFINED HAVE_PTHREAD )
	if(CMAKE_USE_PTHREADS_INIT)
		set(HAVE_PTHREAD 1 CACHE BOOL "Has pthread library.")
	else()
		set(HAVE_PTHREAD 0 CACHE BOOL "Has pthread library.")
	endif()
endif()

if ( HAVE_PTHREAD )
	set(OLD_CMAKE_LIBS ${CMAKE_REQUIRED_LIBRARIES})
	set(CMAKE_REQUIRED_LIBRARIES ${CMAKE_REQUIRED_LIBRARIES} pthread)
	check_cxx_source_compiles("
	#define _XOPEN_SOURCE 500
    #include <pthread.h>
	#ifdef __CYGWIN32__
	# error Cygwin has a bug in pthread_rwlock; disabling
	#endif
	int main() {
		pthread_rwlock_t l; pthread_rwlock_init(&l, NULL);
		pthread_rwlock_rdlock(&l); 
		return 0;
	}"
	HAVE_RWLOCK)
	
	unset(RESULT)
	foreach(attr PTHREAD_CREATE_JOINABLE PTHREAD_CREATE_UNDETACHED )
	    check_cxx_source_compiles("
			#include <pthread.h>
			int main() {
				int attr = ${attr};
				return attr;
			}"
		RESULT)
		if( RESULT )
			if ( NOT ${attr} STREQUAL PTHREAD_CREATE_JOINABLE )
				set(PTHREAD_CREATE_JOINABLE ${attr})
			endif()
			unset(RESULT)
			break()
		endif()
	endforeach()
	set(CMAKE_REQUIRED_LIBRARIES ${OLD_CMAKE_LIBS})
endif()


if( NOT DEFINED google_namespace )
	set(google_namespace "ctemplate" CACHE STRING "Define these Google classes in the FOO namespace. Empty to define them in the global namespace. Default is to define them in namespace ctemplate")	
endif()

if ( google_namespace )
	set(GOOGLE_NAMESPACE "::${google_namespace}")
	set(_START_GOOGLE_NAMESPACE_ "namespace ${google_namespace} {")
	set(_END_GOOGLE_NAMESPACE_ "}")
else()
	set(GOOGLE_NAMESPACE "")
	set(_START_GOOGLE_NAMESPACE_ "")
	set(_END_GOOGLE_NAMESPACE_ "")
endif()

set(ac_google_namespace ${GOOGLE_NAMESPACE})
set(ac_google_start_namespace ${_START_GOOGLE_NAMESPACE_})
set(ac_google_end_namespace ${_END_GOOGLE_NAMESPACE_})

set(ac_cv_cxx_hash_map "<${HASH_MAP_H}>")
set(ac_cv_cxx_hash_set "<${HASH_SET_H}>")
set(ac_cv_cxx_hash_map_class "${HASH_NAMESPACE}::${HASH_TYPE}_map")
set(ac_cv_cxx_hash_set_class "${HASH_NAMESPACE}::${HASH_TYPE}_set")

if ( HAVE___ATTRIBUTE__ )
	set(ac_google_attribute 1)
else()
	set(ac_google_attribute 0)
endif()

normalize_bool(HAVE___ATTRIBUTE__)

if ( HAVE_U_INT64_T )
   set(ac_cv_uint64 "u_int64_t")
elseif( HAVE_UINT64_T )
   set(ac_cv_uint64 "uint64_t")
elseif( HAVE__INT64 )
   set(ac_cv_uint64 "unsigned __int64")
else()
   set(ac_cv_uint64 "unsigned long long")   # best we can do
endif()


include(CheckCSourceRuns)

# One some systems (eg gnu/linux), the linker defines _start and
# data_start to indicate the extent of the .text section.  We can use
# this to know strings are immutable.  In the code, we make the
# variables weak, just in case, but for this check, we only want to
# say "yes" if the linker supports the vars, *and* the compiler supports
# attribute-weak.
check_c_source_compiles("
	int main() {
		extern char _start[];
		extern char data_start[];
		extern char dummy[]  __attribute__((weak));
		return !(_start && data_start && dummy);
	}"
ac_have_attribute_weak)
if ( NOT ac_have_attribute_weak )
	set(ac_have_attribute_weak 0)
endif()

include(CheckCSourceCompiles)
# In unix (that is, using this script), this dll-export stuff will always
# be the empty string.  In windows, we'll replace it with windows-specific
# text.
if ( WIN32 )
	if( BUILD_SHARED_LIBS )
		set(ac_windows_dllexport "CTEMPLATE_DLL_DECL")
		set(ac_windows_dllexport_defines "
// NOTE: if you are statically linking the template library into your binary
// (rather than using the template .dll), set '/D CTEMPLATE_DLL_DECL='
// as a compiler flag in your project file to turn off the dllimports.
#ifndef ${ac_windows_dllexport}
#ifdef ${ac_windows_dllexport}_BUILDING
# define ${ac_windows_dllexport}  __declspec(dllexport)
#else
# define ${ac_windows_dllexport}  __declspec(dllimport)
#endif
#endif
		")
	else()
		set(ac_windows_dllexport_defines "#define CTEMPLATE_DLL_DECL")
	endif()
	set(WINDOWS_EXTRA "
// ---------------------------------------------------------------------
// Extra stuff not found in config.h.in

// This must be defined before anything else in our project: make sure
// that when compiling the dll, we export our functions/classes.  Safe
// to define this here because this file is only used internally, to
// compile the DLL, and every dll source file #includes \"config.h\"
// before anything else.
${ac_windows_dllexport_defines}
// TODO(csilvers): include windows/port.h in every relevant source file instead?
#include \"windows/port.h\"
	")
	
	check_c_source_compiles("
#include <string.h>

int main() {
	char buffer[128];
	snprintf(buffer, sizeof(buffer), \"%s\", \"Hello\");
	return 0;
}"
	HAVE_SNPRINTF)
	normalize_bool(HAVE_SPRINTF)
else()
	set(ac_windows_dllexport_defines " ")
	set(ac_windows_dllexport " ")
endif()

# This will (should) never change, but we put it here so if we do need
# to change it, to avoid naming conflicts or something, it's easy to
# do in one place.
set(ac_htmlparser_namespace google_ctemplate_streamhtmlparser)
if ( NOT DEFINED HTMLPARSER_NAMESPACE)
	set(HTMLPARSER_NAMESPACE ${ac_htmlparser_namespace} CACHE STRING "The namespace to put the htmlparser code.")
endif()

####
# Generate the header files
###
set(HEADER_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/include")
set(GENERATED_FILES
	template_string.h template_enums.h template.h
	template_modifiers.h template_emitter.h template_annotator.h
	template_dictionary.h template_pathops.h template_namelist.h
	per_expand_data.h template_dictionary_interface.h)

#the config.h header is a pretty special guy. Just generate it and don't include it in the
#compiled sources
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/config.h.cmake.in "${CMAKE_CURRENT_SOURCE_DIR}/src/config.h")

foreach (F ${GENERATED_FILES})
	set(IN ${CMAKE_CURRENT_SOURCE_DIR}/src/ctemplate/${F}.in)
	set(OUT "${HEADER_DIRECTORY}/ctemplate/${F}")
	set(ORIGINAL_HEADERS ${ORIGINAL_HEADERS} ${IN})
	set(GENERATED_HEADERS ${GENERATED_HEADERS} ${OUT})
	configure_file(${IN} ${OUT})
endforeach()

include_directories(${HEADER_DIRECTORY} ${CMAKE_CURRENT_SOURCE_DIR}/src)

##
# Note the doc files
##
set(dist_doc_DATA AUTHORS COPYING ChangeLog INSTALL NEWS README README.windows
	doc/designstyle.css doc/index.html
	doc/howto.html doc/tips.html doc/example.html doc/auto_escape.html
	doc/xss_resources.html)

set(SRC_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(CTEMPLATE_SRCS ${GENERATED_HEADERS}
		${SRC_ROOT}/base/arena.h ${SRC_ROOT}/base/arena.cc ${SRC_ROOT}/base/mutex.h
		${SRC_ROOT}/base/small_map.h ${SRC_ROOT}/base/manual_constructor.h
		${SRC_ROOT}/template_modifiers_internal.h ${SRC_ROOT}/indented_writer.h 
		${SRC_ROOT}/template.cc ${SRC_ROOT}/template_dictionary.cc ${SRC_ROOT}/template_modifiers.cc 
		${SRC_ROOT}/template_annotator.cc ${SRC_ROOT}/per_expand_data.cc 
		${SRC_ROOT}/template_string.cc 
		${SRC_ROOT}/template_namelist.cc ${SRC_ROOT}/template_pathops.cc 
		${SRC_ROOT}/htmlparser/htmlparser.h ${SRC_ROOT}/htmlparser/htmlparser.cc 
		${SRC_ROOT}/htmlparser/jsparser.h ${SRC_ROOT}/htmlparser/jsparser.cc 
		${SRC_ROOT}/htmlparser/statemachine.h ${SRC_ROOT}/htmlparser/statemachine.cc 
		${SRC_ROOT}/htmlparser/htmlparser_cpp.h
)

if ( WIN32 )
	set(CTEMPLATE_SRCS ${CTEMPLATE_SRCS} ${SRC_ROOT}/windows/port.cc)
	set(COMPILE_DEFINITIONS ${COMPILE_DEFINITIONS} "${ac_windows_dllexport}_BUILDING")
endif()

add_library(ctemplate ${CTEMPLATE_SRCS})
target_link_libraries(ctemplate ${CMAKE_THREAD_LIBS_INIT})