# vim: set ts=8 :

ACLOCAL_AMFLAGS = -I m4

AM_CPPFLAGS = -Werror -Wall \
	      -DSYSCONFDIR=\"${sysconfdir}/genesis/\" \
	      -DLIBEXECDIR=\"${libexecdir}/genesis/\"

sbin_PROGRAMS = src/genesis

noinst_LTLIBRARIES = src/actions/libaction.la \
		     src/events/libevents.la  \
		     $(NULL)

libexecprogdir = $(libexecdir)/genesis
libexecprog_SCRIPTS = scripts/*.sh

src_actions_libaction_la_SOURCES = src/actions/action.hh         \
				   src/actions/bash-action.cc    \
				   src/actions/bash-action.hh    \
				   src/actions/genesis-action.cc \
				   src/actions/genesis-action.hh \
				   $(NULL)

src_actions_libaction_la_CPPFLAGS = $(AM_CPPFLAGS) -I $(top_srcdir)/src -I $(top_srcdir)/src/actions

src_events_libevents_la_SOURCES = src/events/events.hh         \
				  src/events/genesis-fifo.cc   \
				  src/events/genesis-fifo.hh   \
				  src/events/netlink-route.cc  \
				  src/events/netlink-route.hh  \
				  src/events/netlink-uevent.cc \
				  src/events/netlink-uevent.hh \
				  $(NULL)

src_events_libevents_la_CPPFLAGS = $(AM_CPPFLAGS) -I $(top_srcdir)/src -I $(top_srcdir)/src/events
src_events_libevents_la_CXXFLAGS = $(PCREPLUSPLUS_CFLAGS)

src_genesis_SOURCES = src/bash.cc           \
		      src/bash.hh           \
		      src/config.cc         \
		      src/config.hh         \
		      src/event-listener.cc \
		      src/event-listener.hh \
		      src/genesis.cc        \
		      src/logger.cc         \
		      src/logger.hh         \
		      $(NULL)

src_genesis_CPPFLAGS = $(AM_CPPFLAGS) -I $(top_srcdir)/src
src_genesis_CXXFLAGS = $(PCREPLUSPLUS_CFLAGS)
src_genesis_LDFLAGS = @LIBSTDCXX_A@ $(patsubst .so,.a,$(PCREPLUSPLUS_LIBS))

src_genesis_LINK = $(LIBTOOL) $(AM_V_lt) --tag=CXX --mode=link $(CC) -all-static $(src_genesis_LDFLAGS) -o $@
src_genesis_LDADD = src/events/libevents.la src/actions/libaction.la

