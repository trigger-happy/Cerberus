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
include(CheckIncludeFile)

macro(CHECK_DIRENT)
	check_include_file(dirent.h		HAVE_DIRENT_H)
	check_include_file(sys/ndir.h	HAVE_SYS_NDIR_H) 
	check_include_file(sys/dir.h	HAVE_SYS_DIR_H)
	check_include_file(ndir.h		HAVE_NDIR_H)
endmacro(CHECK_DIRENT)