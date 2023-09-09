## ------------------- ##
#
## ------------------- ##
AUTOMAKE_OPTIONS = 1.4 no-dependencies foreign

## ------------------- ##
# 
## ------------------- ##
ACLOCAL_AMFLAGS = -I m4 

## ------------------- ##
## subdirs of project  ## 
## ------------------- ##
SUBDIRS =   src \
            include

# bin_SCRIPTS = cppunit-config
# man_MANS = cppunit-config.1

#EXTRA_DIST = BUGS INSTALL-unix                          \
#        $(m4sources)

## ------------------- ##
##                     ##
## ------------------- ##
EXTRA_DIST = acinclude.m4                               \
            $(m4sources)


# 
# create m4 dir later, then move in following files:
# ltmain-sh, config.guess, missing, ...
#
m4sources = \
        m4/ac_create_prefix_config_h.m4             \
        m4/ac_cxx_have_sstream.m4                   \
        m4/ac_cxx_have_strstream.m4                 \
        m4/ac_cxx_namespaces.m4                     \
        m4/ac_cxx_rtti.m4                           \
        m4/ac_cxx_string_compare_string_first.m4    \
        m4/cppunit.m4
#        m4/bb_enable_doxygen.m4
