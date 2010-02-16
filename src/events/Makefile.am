noinst_LTLIBRARIES = libevents.la
libevents_la_SOURCES = \
	events.hh \
	netlink-uevent.cc netlink-uevent.hh \
	netlink-route.cc netlink-route.hh \
	genesis-fifo.cc genesis-fifo.hh

AM_CPPFLAGS = -Werror -Wall -I${top_srcdir}/src -DSYSCONFDIR=\"${sysconfdir}/genesis/\"
