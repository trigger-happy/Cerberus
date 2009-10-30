# - Check if for (hash/unordered)_(map/set).
# CHECK_CXX_HASH ()
#

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
				foreach(ns std std::tr1)
					check_cxx_source_compiles("
						#include <${location}unordered_map>
						int main() {
							const ${ns}::unordered_map<int,int> t;
							return t.find(5) == t.end();
						}
					"
					HASH_OK)
					if( HASH_OK )
						set(${OUT_MAP_H} "${location}unordered_map")
						set(${OUT_SET_H} "${location}unordered_set")
						set(${OUT_TYPE} "unordered")
						set(${OUT_NAMESPACE} ${ns})
						return()
					endif()
				endforeach()
			endforeach()
		endif()
		
		#now try hash_map
		
		foreach(location "ext/" "")
			foreach(ns __gnu_cxx "" std stdext)
				check_cxx_source_compiles("
					#include <${location}hash_map>
					int main() {
						const ${ns}::hash_map<int,int> t;
						return 0;
					}
				"
				HASH_OK)
				if( HASH_OK )
					set(${OUT_MAP_H} "${location}hash_map")
					set(${OUT_SET_H} "${location}hash_set")
					set(${OUT_TYPE} "hash")
					set(${OUT_NAMESPACE} ${ns})
					return()
				endif()
			endforeach()
		endforeach()
	endfunction()
	
	if ( NOT DEFINED HASH_MAP_H OR NOT DEFINED HASH_SET_H OR NOT DEFINED HASH_NAMSEPACE OR NOT DEFINED HASH_TYPE )
		check_cxx_hash_container(HASH_MAP_H HASH_SET_H HASH_NAMESPACE HASH_TYPE)
		message(STATUS "Hash found at ${HASH_MAP_H}")
		set(HASH_MAP_H ${HASH_MAP_H} CACHE PATH "Location of hash map header")
		set(HASH_SET_H ${HASH_SET_H} CACHE PATH "Location of hash set header")
		set(HASH_NAMESPACE ${HASH_NAMESPACE} CACHE STRING "Namespace of hash containers")
		set(HASH_TYPE ${HASH_TYPE} CACHE STRING "'unordered' or 'hash'")
		
		if( HASH_TYPE STREQUAL "hash" )
			set(HAVE_HASH_MAP 1)
			set(HAVE_HASH_SET 1)
		endif()
	endif()
endmacro()