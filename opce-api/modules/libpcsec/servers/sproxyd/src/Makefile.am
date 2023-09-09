## ------------------- ##
# 
## ------------------- ##

## Process this file with automake to produce Makefile.in.

bin_PROGRAMS = sproxyd
sproxyd_SOURCES = \
            sproxyd.cc
            
sproxyd_LDADD = $(SKLIBPATH) -lpcap -lsilk -lresolv -lpcff -lpthread -L$(top_builddir)/modules/libpcsec/src/.libs -lpcsec

localedir = $(datadir)/locale

INCLUDES = -I. -I$(srcdir) -I$(top_builddir)/modules/libpcsec/include -I.. -I../..
DEFS = -DLOCALEDIR=\"$(localedir)\" @DEFS@

## # Automake cannot handle this now (version 1.0).
## $(bin_PROGRAMS): $(LDADD)

## src/Makefile.am ends here
