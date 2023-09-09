## ------------------- ##
# 
## ------------------- ##

## Process this file with automake to produce Makefile.in.

bin_PROGRAMS = pcdao_test
pcdao_test_SOURCES = \
           pcdao_test.cc \
           TestDatabase.cc \
           TestDao.cc \
           Test.cc \
           TestList.cc
            
pcdao_test_LDADD = -lpcff -lpthread -L$(top_builddir)/modules/libpcdao/src/.libs -lpcdao $(PQLIBPATH) -lpq

localedir = $(datadir)/locale

INCLUDES = -I. -I$(srcdir) -I$(top_builddir)/modules/libpcdao/include -I.. -I../.. $(PQINCPATH)

DEFS = -DLOCALEDIR=\"$(localedir)\" @DEFS@

## # Automake cannot handle this now (version 1.0).
## $(bin_PROGRAMS): $(LDADD)

## src/Makefile.am ends here
