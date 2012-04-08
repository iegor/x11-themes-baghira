AM_CPPFLAGS = -DQT_PLUGIN

INCLUDES = -I$(top_srcdir)/kdefx $(all_includes)
noinst_HEADERS = baghira.h pixmaps.h bitmaps.h
METASOURCES = AUTO
kde_style_LTLIBRARIES = baghira.la
baghira_la_LDFLAGS = $(all_libraries) $(KDE_PLUGIN) -module
baghira_la_LIBADD = -lkdefx -lXtst $(LIB_KDEUI)
baghira_la_SOURCES = baghira.cpp optionHandler.cpp polish.cpp utils.cpp
lnkdir = $(kde_datadir)/kstyle/themes
lnk_DATA = baghira.themerc
EXTRA_DIST = $(lnk_DATA)
pixmaps.h: $(srcdir)/generatePixmaps.sh
	$(SHELL) $(srcdir)/generatePixmaps.sh $(top_srcdir)
CLEANFILES = pixmaps.h
baghira.lo: pixmaps.h
