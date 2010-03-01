# vim: set sw=4 ts=8 :

ACLOCAL_AMFLAGS = -I m4

AM_CPPFLAGS = -Werror -Wall                           \
	      -I $(top_srcdir)/src                    \
	      -DSYSCONFDIR=\"${sysconfdir}/genesis/\" \
	      -DLIBEXECDIR=\"${libexecdir}/genesis/\"

sbin_PROGRAMS = src/genesis-handler/genesis-handler

noinst_LTLIBRARIES = src/actions/libaction.la               \
		     src/event-sources/libevent-sources.la  \
		     src/util/libutil.la                    \
		     $(NULL)

libexecprogdir = $(libexecdir)/genesis
libexecprog_SCRIPTS = scripts/*.sh

src_util_libutil_la_SOURCES = src/util/singleton.hh        \
			      src/util/log-destination .hh \
			      src/util/log.cc              \
			      src/util/log.hh              \
			      $(NULL)

src_actions_libaction_la_SOURCES = src/actions/action.hh         \
				   src/actions/bash-action.cc    \
				   src/actions/bash-action.hh    \
				   src/actions/genesis-action.cc \
				   src/actions/genesis-action.hh \
				   $(NULL)

src_actions_libaction_la_LIBADD = src/util/libutil.la

src_event_sources_libevent_sources_la_SOURCES = src/event-sources/events.hh         \
						src/event-sources/genesis-fifo.cc   \
						src/event-sources/genesis-fifo.hh   \
						src/event-sources/netlink-route.cc  \
						src/event-sources/netlink-route.hh  \
						src/event-sources/netlink-uevent.cc \
						src/event-sources/netlink-uevent.hh \
						$(NULL)

src_event_sources_libevent_sources_la_CXXFLAGS = $(PCREPLUSPLUS_CFLAGS)
src_event_sources_libevent_sources_la_LDFLAGS = $(PCREPLUSPLUS_LIBS)
src_event_sources_libevent_sources_la_LIBADD = src/util/libutil.la src/actions/libaction.la

src_genesis_handler_genesis_handler_SOURCES = src/genesis-handler/bash.cc            \
					      src/genesis-handler/bash.hh            \
					      src/genesis-handler/config.cc          \
					      src/genesis-handler/config.hh          \
					      src/genesis-handler/event-listener.cc  \
					      src/genesis-handler/event-listener.hh  \
					      src/genesis-handler/genesis-handler.cc \
					      $(NULL)

src_genesis_handler_genesis_handler_CXXFLAGS = $(PCREPLUSPLUS_CFLAGS)
src_genesis_handler_genesis_handler_LDFLAGS = -all-static $(PCREPLUSPLUS_LIBS)
src_genesis_handler_genesis_handler_LDADD = src/util/libutil.la src/event-sources/libevent-sources.la

MAINTAINERCLEANFILES = aclocal.m4 ChangeLog config.guess config.sub configure depcomp install-sh ltmain.sh Makefile.in missing

