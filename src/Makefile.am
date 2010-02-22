SUBDIRS = actions events

sbin_PROGRAMS = genesis
genesis_SOURCES = \
	bash.cc bash.hh \
	config.cc config.hh \
	genesis.cc \
	utils.cc utils.hh

LDADD = events/libevents.la actions/libaction.la

AM_CPPFLAGS = -Werror -Wall \
	-DSYSCONFDIR=\"${sysconfdir}/genesis/\" \
	-DLIBEXECDIR=\"${libexecdir}/genesis/\"
