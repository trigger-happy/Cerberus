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

function(CHECK_TYPES_COMPATIBLE TYPE_A TYPE_B RET)
	check_c_source_compiles("
	#include <stddef.h>
	int main() {
		${TYPE_A} v1 = 0;
		${TYPE_B} v2 = 0;
		return (&v1 - &v2);
	}
	"
	TYPE_CHECK)
	set(${RET} ${TYPE_CHECK} PARENT_SCOPE)
	#message("HERE! ${TYPE_A} ~ ${TYPE_B} = ${TYPE_CHECK} == ${RET}")
endfunction()

function(GET_PRIUS_PREFIX OUT_PREFIX)
	unset(${OUT_PREFIX})
	
	CHECK_TYPES_COMPATIBLE("unsigned int" "size_t", RESULT)
	if( RESULT )
		set(${OUT_PREFIX} "" PARENT_SCOPE)
	endif()
	
	unset(RESULT)
	CHECK_TYPES_COMPATIBLE("unsigned long" "size_t" RESULT)
	if( RESULT )
		set(${OUT_PREFIX} "l" PARENT_SCOPE)
	endif()
	
	unset(RESULT)
	CHECK_TYPES_COMPATIBLE("unsigned long long" "size_t" RESULT)
	if( RESULT )
		set(${OUT_PREFIX} "ll" PARENT_SCOPE)
	endif()
	
	if ( NOT DEFINED OUT_PREFIX )
		set(${OUT_PREFIX} "z" PARENT_SCOPE)
	endif()
endfunction()

macro(CHECK_COMPILER_CHARACTERISTICS)
	get_prius_prefix(PRIUS_PREFIX)
	set(PRIuS "\"${PRIUS_PREFIX}u\"")
	set(PRIxS "\"${PRIUS_PREFIX}x\"")
	set(PRIdS "\"${PRIUS_PREFIX}d\"")
endmacro()