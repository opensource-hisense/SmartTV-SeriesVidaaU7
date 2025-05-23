#-----------------------------------------------------------------------------
# Copyright (c) 2010, MediaTek Inc.
# All rights reserved.
#
# Unauthorized use, practice, perform, copy, distribution, reproduction,
# or disclosure of this information in whole or in part is prohibited.
#-----------------------------------------------------------------------------
# $RCSfile:  $
# $Revision:
# $Date:
# $Author:  $
# $CCRevision:  $
# $SWAuthor:  $
# $MD5HEX:  $
#
# Description:
#        A set of files describing the framebuffer interface and core types.
#---------------------------------------------------------------------------*/

ifndef VM_LINUX_ROOT
VM_LINUX_ROOT := $(word 1, $(subst /vm_linux/,/vm_linux /, $(shell pwd)))
endif

# For MT5396 above (old)
#ENABLE_CA9 ?= true

# Select tool chain and library
#ifeq "$(ENABLE_VFP)" "true"
#    THIRDPARTY_LIB_ROOT=$(VM_LINUX_ROOT)/third_party/library/gnuarm-$(TOOL_CHAIN)_vfp
#else
#    THIRDPARTY_LIB_ROOT=$(VM_LINUX_ROOT)/third_party/library/gnuarm-$(TOOL_CHAIN)
#endif

include $(VM_LINUX_ROOT)/oss/source/mak/target.mak

SQLITE3_SO=libsqlite_3_17_0.so

SQLLITE_OBJ=sqlite3.o

THIS_DIR := $(shell pwd)

DEBUG_ON=0

ifeq ($(DEBUG_ON), 1)
    CFLAGS = -fPIC -g -mapcs-frame -O0 -fsigned-char -fshort-wchar -Wno-strict-aliasing
else
    CFLAGS = -fPIC -O2 -fsigned-char -fshort-wchar -Wno-strict-aliasing
endif

$(SQLITE3_SO) : $(SQLLITE_OBJ)
	$(CC) -fPIC -shared $(CFLAGS) -o $@ $(SQLLITE_OBJ)

.PHONY: all install clean

all: $(SQLITE3_SO)
	@echo 'Create Sqlite3 library!'

.cpp.o: .cpp
	$(CPP) $(CFLAGS) $< -c -o $@

.c.o: .c
	$(CC) $(CFLAGS) $< -c -o $@
	
clean:
	-rm -f $(SQLLITE_OBJ) $(SQLITE3_SO)

install:
	@echo "Install sqlite3 library. OSS_LIB_ROOT=$(OSS_LIB_ROOT) THIS_DIR=$(THIS_DIR)"
	rm -rf $(OSS_LIB_ROOT)/sqlite3
	mkdir -p $(OSS_LIB_ROOT)/sqlite3/$(SQLITE_VERSION)/include
	mkdir -p $(OSS_LIB_ROOT)/sqlite3/$(SQLITE_VERSION)/lib
	cp Makefile.lib $(OSS_LIB_ROOT)/sqlite3/Makefile
	cp -r $(THIS_DIR)/*.h $(OSS_LIB_ROOT)/sqlite3/$(SQLITE_VERSION)/include
	cp -r $(THIS_DIR)/*.so $(OSS_LIB_ROOT)/sqlite3/$(SQLITE_VERSION)/lib
	@echo 'Release sqlite library finished.'
