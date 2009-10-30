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