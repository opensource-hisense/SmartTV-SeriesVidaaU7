## Makefile.am for DirectFB/src/input

INCLUDES = \
	-I$(top_builddir)/lib		\
	-I$(top_builddir)/include	\
	-I$(top_srcdir)/include		\
	-I$(top_srcdir)/lib		\
	-I$(top_srcdir)/src


internalincludedir = $(INTERNALINCLUDEDIR)/input

internalinclude_HEADERS = \
	idirectfbinputdevice.h	\
	idirectfbinputbuffer.h


noinst_LTLIBRARIES = libdirectfb_input.la

if DIRECTFB_BUILD_PURE_VOODOO
NON_PURE_VOODOO_SOURCESs = 
else
NON_PURE_VOODOO_SOURCESs = \
	idirectfbinputdevice.c
endif

libdirectfb_input_la_SOURCES = \
	$(NON_PURE_VOODOO_SOURCESs)	\
	idirectfbinputbuffer.c

AM_CFLAGS = -fvisibility=default

