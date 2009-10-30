include(CheckIncludeFile)

macro(CHECK_DIRENT)
	check_include_file(dirent.h		HAVE_DIRENT_H)
	check_include_file(sys/ndir.h	HAVE_SYS_NDIR_H) 
	check_include_file(sys/dir.h	HAVE_SYS_DIR_H)
	check_include_file(ndir.h		HAVE_NDIR_H)
endmacro(CHECK_DIRENT)