AUTOMAKE_OPTIONS = foreign

SUBDIRS = config

KDE_CXXFLAGS = -DQT_PLUGIN

INCLUDES = $(all_includes) -I$(kde_includes)/kwin

kwindir = $(kde_datadir)/kwin/
kwin_DATA = baghira.desktop

EXTRA_DIST = $(kwin_DATA) images

noinst_HEADERS = baghiraclient.h pixmaps.h masks.h

kde_module_LTLIBRARIES = kwin3_baghira.la
kwin3_baghira_la_SOURCES = baghiraclient.cc
kwin3_baghira_la_LIBADD = $(kde_libraries)/libkdecorations.la
kwin3_baghira_la_LDFLAGS = $(all_libraries) $(KDE_PLUGIN) -lkdecore -module
kwin3_baghira_la_METASOURCES = AUTO

DISTCLEANFILES = $(kwin3_baghira_la_METASOURCES)

pixmaps.h: $(srcdir)/generatePixmaps.sh
	$(SHELL) $(srcdir)/generatePixmaps.sh $(top_srcdir)
CLEANFILES = pixmaps.h
baghiraclient.lo: pixmaps.h

