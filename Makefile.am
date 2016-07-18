AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS}
AM_CPPFLAGS = $(jack2_CFLAGS) -std=c++11 -pthread
bin_PROGRAMS = aramv0
aramv0_SOURCES = src/main/cc/main.cc src/main/cc/service/system.cc src/main/cc/model/title.cc \
                 src/main/cc/model/track.cc
aramv0_LDADD = $(jack2_LIBS)
aramv0_LDFLAGS = -pthread

noinst_PROGRAMS = testrun
testrun_SOURCES = src/test/cc/main.cc
testrun_LDADD = $(sqlite_LIBS)
testrun_LDFLAGS = -pthread

## dist_noinst_SCRIPTS = autogen.sh
