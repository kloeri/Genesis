# vim: set sw=4 ts=8 :

ACLOCAL_AMFLAGS = -I m4

AM_CPPFLAGS = -Werror -Wall                           \
	      -I $(top_srcdir)/src                    \
	      -I $(top_srcdir)/src/genesis-handler    \
	      -DSYSCONFDIR=\"${sysconfdir}/genesis/\" \
	      -DLIBEXECDIR=\"${libexecdir}/genesis/\"

sbin_PROGRAMS = src/genesis-handler/genesis-handler

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

src_events_libevents_la_SOURCES = src/events/events.hh         \
				  src/events/genesis-fifo.cc   \
				  src/events/genesis-fifo.hh   \
				  src/events/netlink-route.cc  \
				  src/events/netlink-route.hh  \
				  src/events/netlink-uevent.cc \
				  src/events/netlink-uevent.hh \
				  $(NULL)

src_events_libevents_la_CXXFLAGS = $(PCREPLUSPLUS_CFLAGS)

src_genesis_handler_genesis_handler_SOURCES = src/genesis-handler/bash.cc            \
					      src/genesis-handler/bash.hh            \
					      src/genesis-handler/config.cc          \
					      src/genesis-handler/config.hh          \
					      src/genesis-handler/event-listener.cc  \
					      src/genesis-handler/event-listener.hh  \
					      src/genesis-handler/genesis-handler.cc \
					      src/genesis-handler/logger.cc          \
					      src/genesis-handler/logger.hh          \
					      src/genesis-handler/singleton.hh       \
					      $(NULL)

src_genesis_handler_genesis_handler_CXXFLAGS = $(PCREPLUSPLUS_CFLAGS)
src_genesis_handler_genesis_handler_LDFLAGS = -all-static $(PCREPLUSPLUS_LIBS)
src_genesis_handler_genesis_handler_LDADD = src/events/libevents.la src/actions/libaction.la

MAINTAINERCLEANFILES = aclocal.m4 ChangeLog config.guess config.sub configure depcomp install-sh ltmain.sh Makefile.in missing

