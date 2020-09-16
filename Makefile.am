lib_LTLIBRARIES = retroreflector.la

retroreflector_la_SOURCES = retroreflector.cpp
retroreflector_la_CPPFLAGS = -I$(top_srcdir)/include
retroreflector_la_LDFLAGS = -module -avoid-version -shared

AM_CPPFLAGS = $(CONF_CPPFLAGS)
AM_CFLAGS = $(CONF_CFLAGS)
AM_CXXFLAGS = $(CONF_CXXFLAGS)

EXTRA_DIST = \
	README.md \
	retroreflector.sln \
	retroreflector.vcxproj	\
	retroreflector.vcxproj.filters

MAINTAINERCLEANFILES =	\
	Makefile.in
