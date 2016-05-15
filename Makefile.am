# vim: set sw=4 ts=8 :

ACLOCAL_AMFLAGS = -I m4

AM_CPPFLAGS = -Wall                                   \
	      -I $(top_srcdir)/src                    \
	      -DSYSCONFDIR=\"${sysconfdir}/genesis/\" \
	      -DLIBEXECDIR=\"${libexecdir}/genesis/\"

sbin_PROGRAMS = src/genesis-handler/genesis-handler \
		src/genesis/genesis                 \
		$(NULL)

noinst_LTLIBRARIES = src/actions/libaction.la               \
		     src/event-sources/libevent-sources.la  \
		     src/util/libutil.la                    \
		     src/jobs/libjobs.la                    \
		     $(NULL)

libexecprogdir = $(libexecdir)/genesis
libexecprog_SCRIPTS = scripts/bash-run.sh       \
		      scripts/netlink-uevent.sh \
		      $(NULL)

src_util_libutil_la_SOURCES = src/util/singleton.hh        \
			      src/util/log-destinations.hh \
			      src/util/log.cc              \
			      src/util/log.hh              \
			      src/util/tokenise.cc         \
			      src/util/tokenise.hh         \
			      $(NULL)

src_util_libutil_la_CXXFLAGS = $(PCREPLUSPLUS_CFLAGS)
src_util_libutil_la_LDFLAGS = $(PCREPLUSPLUS_LIBS)

src_actions_libaction_la_SOURCES = src/actions/action.hh         \
				   src/actions/bash-action.cc    \
				   src/actions/bash-action.hh    \
				   src/actions/genesis-action.cc \
				   src/actions/genesis-action.hh \
				   $(NULL)

src_actions_libaction_la_CXXFLAGS = $(PCREPLUSPLUS_CFLAGS)
src_actions_libaction_la_LDFLAGS = $(PCREPLUSPLUS_LIBS)
src_actions_libaction_la_LIBADD = src/util/libutil.la

src_event_sources_libevent_sources_la_SOURCES = src/event-sources/event.hh          \
						src/event-sources/event-source.cc   \
						src/event-sources/event-source.hh   \
						src/event-sources/genesis-pipe.cc   \
						src/event-sources/genesis-pipe.hh   \
						src/event-sources/netlink-route.cc  \
						src/event-sources/netlink-route.hh  \
						src/event-sources/netlink-uevent.cc \
						src/event-sources/netlink-uevent.hh \
						$(NULL)

src_event_sources_libevent_sources_la_CXXFLAGS = $(PCREPLUSPLUS_CFLAGS)
src_event_sources_libevent_sources_la_LDFLAGS = $(PCREPLUSPLUS_LIBS)
src_event_sources_libevent_sources_la_LIBADD = src/util/libutil.la src/actions/libaction.la

src_jobs_libjobs_la_SOURCES = src/jobs/jobs.hh         \
			      src/jobs/jobs.cc    \
			      $(NULL)

src_jobs_libjobs_la_CXXFLAGS = $(PCREPLUSPLUS_CFLAGS)
src_jobs_libjobs_la_LDFLAGS = $(PCREPLUSPLUS_LIBS)

src_genesis_handler_genesis_handler_SOURCES = src/genesis-handler/bash.cc            \
					      src/genesis-handler/bash.hh            \
					      src/genesis-handler/config.cc          \
					      src/genesis-handler/config.hh          \
					      src/genesis-handler/event-listener.cc  \
					      src/genesis-handler/event-listener.hh  \
					      src/genesis-handler/genesis-handler.cc \
					      $(NULL)

src_genesis_handler_genesis_handler_CXXFLAGS = $(PCREPLUSPLUS_CFLAGS)
src_genesis_handler_genesis_handler_LDFLAGS = $(PCREPLUSPLUS_LIBS)
src_genesis_handler_genesis_handler_LDADD = src/util/libutil.la src/event-sources/libevent-sources.la

src_genesis_genesis_SOURCES = src/genesis/argument.cc             \
			      src/genesis/argument.hh             \
			      src/genesis/command_line.cc         \
			      src/genesis/command_line.hh         \
			      src/genesis/genesis.cc              \
			      src/genesis/genesis_command_line.cc \
			      src/genesis/genesis_command_line.hh \
			      $(NULL)

EXTRA_DIST = scripts/bash-run.sh       \
	     scripts/netlink-uevent.sh \
	     $(NULL)

MAINTAINERCLEANFILES = aclocal.m4 ChangeLog config.guess config.sub configure depcomp install-sh ltmain.sh Makefile.in missing

