## ------------------- ##
# 
## ------------------- ##

## Process this file with automake to produce Makefile.in.

bin_PROGRAMS = silkproxyd
silkproxyd_SOURCES = \
            SilkSensorProxy.cc \
            SilkSensorProxyConfig.cc \
            SilkSensorProxyConfigCommandLine.cc \
            SilkSensorProxyConfigFile.cc \
            SilkSensorProxyException.cc \
            silkproxyd.cc
            
silkproxyd_LDADD = -lpcff $(SKLIBPATH) -lpcap -lsilk -lresolv -lpthread -L$(top_builddir)/modules/libpcsec/src/.libs -lpcsec

localedir = $(datadir)/locale

INCLUDES = -I. $(SKINCPATH) -I$(srcdir) -I$(top_builddir)/modules/libpcsec/include -I.. -I../..
DEFS = -DLOCALEDIR=\"$(localedir)\" @DEFS@

## # Automake cannot handle this now (version 1.0).
## $(bin_PROGRAMS): $(LDADD)

## src/Makefile.am ends here
