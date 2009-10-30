include(CheckCSourceCompiles)

macro(CHECK_HAVE_ATTRIBUTE)
	if ( NOT DEFINED HAVE___ATTRIBUTE__ )
		check_c_source_compiles("
		#include <stdlib.h>
		static void foo(void) __attribute__ ((unused));
		void foo(void) { exit(1); }
		int main() {
			return 0;
		}"
		CHECK_HAVE_ATTRIBUTE_RESULT)
		
		if ( CHECK_HAVE_ATTRIBUTE_RESULT )
			set(CHECK_HAVE_ATTRIBUTE_RESULT 1)
		else()
			set(CHECK_HAVE_ATTRIBUTE_RESULT 0)
		endif()
		set(HAVE___ATTRIBUTE__ ${CHECK_HAVE_ATTRIBUTE_RESULT} CACHE BOOL "Compiler supports __attribute__.")
		unset(CHECK_HAVE_ATTRIBUTE_RESULT)
	endif()
endmacro()