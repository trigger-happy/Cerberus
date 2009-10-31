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

# - Check if for (hash/unordered)_(map/set).
# CHECK_CXX_HASH()
# defines the following: HASH_MAP_H HASH_SET_H HASH_NAMESPACE HASH_TYPE
# Original logic from stl_hash.m4

include(CheckCXXSourceCompiles)

macro(CHECK_CXX_HASH)
	function(CHECK_CXX_HASH_CONTAINER OUT_MAP_H OUT_SET_H OUT_NAMESPACE OUT_TYPE)
		check_cxx_source_compiles("
			#if defined(__GNUC__) && (__GNUC__ < 4 || (__GNUC__ == 4 && __GNUC_MINOR__ < 2))
				# error GCC too old for unordered_map
			#endif
			int main() {
				return 0;
			}
		"
		STL_HASH_NEW_GCC)
		if( STL_HASH_NEW_GCC )
			foreach(location "" "tr1/")
				foreach(ns std "std::tr1")
					message("Trying ${ns}::unordered_map in <${location}unordered_map>")
					set(HASH_VAR "HASH_${ns}_${location}unordered_map")
					check_cxx_source_compiles("
						#include <${location}unordered_map>
						int main() {
							const ${ns}::unordered_map<int,int> t;
							return t.find(5) == t.end();
						}
					"
					${HASH_VAR})
					if( ${HASH_VAR} )
						set(${OUT_MAP_H} "${location}unordered_map" PARENT_SCOPE)
						set(${OUT_SET_H} "${location}unordered_set" PARENT_SCOPE)
						set(${OUT_TYPE} "unordered" PARENT_SCOPE)
						set(${OUT_NAMESPACE} ${ns} PARENT_SCOPE)
						return()
					endif()
				endforeach()
			endforeach()
		endif()
		
		#now try hash_map
		
		foreach(location "ext/" "")
			foreach(ns __gnu_cxx "" std stdext)
				message("Trying ${ns}::hash_map in <${location}hash_map>")
				set(HASH_VAR "HASH_${ns}_${location}hash_map")
				check_cxx_source_compiles("
					#include <${location}hash_map>
					int main() {
						const ${ns}::hash_map<int,int> t;
						return 0;
					}
				"
				${HASH_VAR})
				if( ${HASH_VAR} )
					set(${OUT_MAP_H} "${location}hash_map" PARENT_SCOPE)
					set(${OUT_SET_H} "${location}hash_set" PARENT_SCOPE)
					set(${OUT_TYPE} "hash" PARENT_SCOPE)
					set(${OUT_NAMESPACE} ${ns} PARENT_SCOPE)
					return()
				endif()
			endforeach()
		endforeach()
		message(SEND_ERROR "No hash container for C++ found.")
	endfunction()
	
	if ( NOT DEFINED HASH_MAP_H OR NOT DEFINED HASH_SET_H OR NOT DEFINED HASH_NAMSEPACE OR NOT DEFINED HASH_TYPE )
		check_cxx_hash_container(HASH_MAP_H HASH_SET_H HASH_NAMESPACE HASH_TYPE)
		message(STATUS "${HASH_TYPE} map container found at ${HASH_MAP_H} in namespace ${HASH_NAMESPACE}")
		set(HASH_MAP_H ${HASH_MAP_H} CACHE PATH "Location of hash map header")
		set(HASH_SET_H ${HASH_SET_H} CACHE PATH "Location of hash set header")
		set(HASH_NAMESPACE ${HASH_NAMESPACE} CACHE STRING "Namespace of hash containers")
		set(HASH_TYPE ${HASH_TYPE} CACHE STRING "'unordered' or 'hash'")
		
		if( HASH_TYPE STREQUAL "hash" )
			set(HAVE_HASH_MAP 1)
			set(HAVE_HASH_SET 1)
		else()
			set(HAVE_UNORDERED_MAP 1)
			set(HAVE_UNORDERED_SET 1)
		endif()
	endif()
endmacro()