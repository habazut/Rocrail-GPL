#    Rocrail Copyright (C) 2002 - 2007 Rob Versluis 
#
#    This program is free software; you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation; either version 2 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program; if not, write to the Free Software
#    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA


# commandline for mingw cross compiling:
#   make clean
#   make TOOLPREFIX=i586-mingw32msvc- LIBSUFFIX=-i586-mingw32msvc PLATFORM=WIN32 MINGWINSTALL=/usr/i586-mingw32msvc all
#
# commandline for osx compiling:
#   make PLATFORM=MACOSX
#
FS=/
CS=;
COPY=cp
REMOVE=rm -f
GENSUFFIX=
QUOT="
MAKE ?= make

ifeq ($(NATIVE),WIN32)
	FS=$\\
	CS=&
	COPY=copy
	REMOVE=del /Q
	GENSUFFIX=.exe
	QUOT=
endif



PLATFORM=LINUX
ARCH=

offlineall: version
	cd rocrail$(CS) $(MAKE) rocrail TOOLPREFIX=$(TOOLPREFIX) LIBSUFFIX=$(LIBSUFFIX) PLATFORM=$(PLATFORM) ARCH=$(ARCH) NATIVE=$(NATIVE) MINGWINSTALL=$(MINGWINSTALL)

all: version
	cd rocrail$(CS) $(MAKE) rocrail TOOLPREFIX=$(TOOLPREFIX) LIBSUFFIX=$(LIBSUFFIX) PLATFORM=$(PLATFORM) ARCH=$(ARCH) NATIVE=$(NATIVE) MINGWINSTALL=$(MINGWINSTALL)

fromtar: version
	cd rocrail$(CS) $(MAKE) fromtar TOOLPREFIX=$(TOOLPREFIX) LIBSUFFIX=$(LIBSUFFIX) PLATFORM=$(PLATFORM) ARCH=$(ARCH) NATIVE=$(NATIVE) MINGWINSTALL=$(MINGWINSTALL)

release: version
	cd rocrail$(CS) $(MAKE) rocrail TOOLPREFIX=$(TOOLPREFIX) LIBSUFFIX=$(LIBSUFFIX) PLATFORM=$(PLATFORM) ARCH=$(ARCH) NATIVE=$(NATIVE) MINGWINSTALL=$(MINGWINSTALL) DEBUG=
	
install:
	cd rocrail$(CS) $(MAKE) install_all

uninstall:
	cd rocrail$(CS) $(MAKE) uninstall_all


version:
	echo $(QUOT)const int bzr = $(QUOT) > common$(FS)version.h
	bzr revno >> common$(FS)version.h
	echo $(QUOT);$(QUOT) >> common$(FS)version.h

