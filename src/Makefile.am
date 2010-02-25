
AM_CPPFLAGS = -Werror -Wall \
	      -DSYSCONFDIR=\"${sysconfdir}/genesis/\" \
	      -DLIBEXECDIR=\"${libexecdir}/genesis/\"

AM_CXXFLAGS = $(PCREPLUSPLUS_CFLAGS)

SUBDIRS = actions events

sbin_PROGRAMS = genesis

genesis_SOURCES = \
	bash.cc bash.hh \
	config.cc config.hh \
	genesis.cc \
	logger.cc logger.hh \
	utils.cc utils.hh

genesis_LINK = $(LIBTOOL) --tag=CXX --mode=link $(CC) -all-static $(genesis_LDFLAGS) -o $@
genesis_LDADD = events/libevents.la actions/libaction.la
genesis_LDFLAGS = @LIBSTDCXX_A@ $(patsubst .so,.a,$(PCREPLUSPLUS_LIBS))

