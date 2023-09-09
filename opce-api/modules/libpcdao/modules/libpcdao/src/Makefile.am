## ------------------------ ##
## includes                 ##
## ------------------------ ##
#      -I$(top_builddir)/modules/include/opce
#      -I$(top_srcdir)/include

INCLUDES =  -I ../include/opce \
	    -I ../../../../libpcff/modules/libpcff/include/opce \
      $(PQINCPATH)

## ------------------------ ##
## library name             ##
## ------------------------ ##
lib_LTLIBRARIES = libpcdao.la

## ------------------------ ##
## sources                  ##
## ------------------------ ##
libpcdao_la_SOURCES = \
                driver/pgsql/PgSqlConnection.cc \
                driver/pgsql/PgSqlCursor.cc \
                driver/pgsql/PgSqlResult.cc \
                driver/pgsql/PgSqlTools.cc \
                driver/pgsql/PgSqlTransaction.cc \
                Connection.cc \
                ConnectionDirector.cc \
                Cursor.cc \
                DaoConfiguration.cc \
                DaoConnectionException.cc \
                DaoObject.cc \
                DaoObjectList.cc \
                DaoResultException.cc \
                Database.cc \
                Transaction.cc

## ------------------------ ##
## LDFLAGS used to create   ##
## versioning suffix        ##
## defined in configure.ac  ##
## ------------------------ ##
libpcdao_la_LDFLAGS= \
-version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) \
-release $(LT_RELEASE)

