AUTOMAKE_OPTIONS = foreign

KDE_CXXFLAGS = -DQT_PLUGIN

INCLUDES = $(all_includes) -I$(srcdir)/../../config

noinst_HEADERS = baghiraconfig.h aquariusbutton.h colorpicker.h pixmaps.h

kde_module_LTLIBRARIES = kwin_baghira_config.la
kwin_baghira_config_la_SOURCES = baghiraconfig.cc aquariusbutton.cc colorpicker.cc configdialog.ui
kwin_baghira_config_la_LDFLAGS = $(all_libraries) $(KDE_PLUGIN) -module
kwin_baghira_config_la_LIBADD = $(LIB_KDEUI)
kwin_baghira_config_la_METASOURCES = AUTO

DISTCLEANFILES = $(kwin_baghira_config_la_METASOURCES)

messages:
	$(XGETTEXT) *.cpp -o $(podir)/kwin_baghira_config.pot

pixmaps.h: $(srcdir)/generatePixmaps.sh
	$(SHELL) $(srcdir)/generatePixmaps.sh $(top_srcdir)
CLEANFILES = pixmaps.h
baghiraconfig.lo: pixmaps.h
