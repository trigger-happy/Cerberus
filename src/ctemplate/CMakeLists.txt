cmake_minimum_required(VERSION 2.6)

project("ctemplate")
set(PACKAGE_NAME "ctemplate")
set(PACKAGE_VERSION  VERSION 0.96)
set(PACKAGE_BUGREPORT "opensource@google.com")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "CMakeModules")

set(CTEMPLATE_SRCS)

include(CheckFunctionExists)
include(CheckTypeSize)
include(CheckIncludeFile)
include(CheckLibraryExists)
include(CheckCxxSourceCompiles)
include(CheckCXXHash)
include(FindThreads)
include(CheckDirent)
include(CheckCompilerCharacteristics)
include(CheckHaveAttribute)

check_function_exists(getopt_long HAVE_GETOPT_LONG)

# These are the types I need.  We look for them in either stdint.h,
# sys/types.h, or inttypes.h, all of which are part of the default-includes.
check_type_size(uint32_t UINT32_T)
check_type_size(u_int32_t U_INT32_T)
check_type_size(__int32 __INT32)
check_type_size(uint64_t UINT64_T)
check_type_size(u_int64_t U_INT64_T)
check_type_size(__int64 __INT64)

check_include_file(inttypes.h HAVE_INTTYPES_H)
check_include_file(dlfcn.h HAVE_DLFCN_H)
check_dirent()
check_include_file(sys/stat.h HAVE_SYS_STAT_H)
check_include_file(sys/types.h HAVE_SYS_TYPES_H)
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

if(CMAKE_USE_PTHREADS_INIT)
	set(HAVE_PTHREAD 1)
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
	    check_cxx_source_runs("
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
endif(CMAKE_USE_PTHREADS_INIT)


if( NOT DEFINED google_namespace )
	set(google_namespace "ctemplate" CACHE STRING "Define these Google classes in the FOO namespace. Empty to define them in the global namespace. Default is to define them in namespace ctemplate")	
endif()

if ( use_namespace )
	set(GOOGLE_NAMESPACE "::${google_namespace}")
	set(_START_GOOGLE_NAMESPACE_ "namespace ${google_namespace} {")
	set(_END_GOOGLE_NAMESPACE_ "}")
else()
	set(GOOGLE_NAMESPACE "")
	set(_START_GOOGLE_NAMESPACE_ "")
	set(_END_GOOGLE_NAMESPACE_ "")
endif()

set(ac_cv_cxx_hash_map "<${HASH_MAP_H}>")
set(ac_cv_cxx_hash_set "<${HASH_SET_H}>")
set(ac_cv_cxx_hash_map_class "${HASH_NAMESPACE}::${HASH_TYPE}_map")
set(ac_cv_cxx_hash_set_class "${HASH_NAMESPACE}::${HASH_TYPE}_set")

if ( HAVE___ATTRIBUTE__ )
	set(ac_google_attribute 1)
else()
	set(ac_google_attribute 0)
endif()

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
check_c_source_runs("
	int main() {
		extern char _start[];
		extern char data_start[];
		extern char dummy[]  __attribute__((weak));
		return (_start && data_start && dummy);
	}"
ac_have_attribute_weak)
if ( NOT ac_have_attribute_weak )
	set(ac_have_attribute_weak 0)
endif()

# In unix (that is, using this script), this dll-export stuff will always
# be the empty string.  In windows, we'll replace it with windows-specific
# text.
if ( MSVC )
	set(ac_windows_dllexport "CTEMPLATE_DLL_DECL")
	set(ac_windows_dllexport_defines "
		// NOTE: if you are statically linking the template library into your binary
		// (rather than using the template .dll), set '/D CTEMPLATE_DLL_DECL='
		// as a compiler flag in your project file to turn off the dllimports.
		#ifndef ${ac_windows_dllexport}
		# define ${ac_windows_dllexport}  __declspec(dllimport)
		#endif
	")
else()
	set(ac_windows_dllexport_defines "")
	set(ac_windows_dllexport "")
endif()

# This will (should) never change, but we put it here so if we do need
# to change it, to avoid naming conflicts or something, it's easy to
# do in one place.
set(ac_htmlparser_namespace google_ctemplate_streamhtmlparser)
if ( NOT DEFINED HTMLPARSER_NAMESPACE)
	set(HTMLPARSER_NAMESPACE ${ac_htmlparser_namespace} CACHE STRING "The namespace to put the htmlparser code.")
endif()

#configure_file("src/config.h.cmake.in" "src/config.h")

#add_library(ctemplate ${CTEMPLATE_SRCS})