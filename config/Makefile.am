INCLUDES    = $(all_includes)
noinst_HEADERS = about.h configdialog.h help.h kstyle_baghira_config.h colordialog.h colorpicker.h
METASOURCES = AUTO

# Install this plugin in the KDE modules directory
kde_module_LTLIBRARIES = kstyle_baghira_config.la

kstyle_baghira_config_la_SOURCES = kstyle_baghira_config.cpp colordialog.cpp colorpicker.cpp configdialog.ui help.ui about.ui
# kstyle_baghira_config_la_METASOURCES = AUTO
kstyle_baghira_config_la_LIBADD = $(LIB_KDEUI) $(LIB_KIO)
kstyle_baghira_config_la_LDFLAGS = -module -avoid-version $(all_libraries) -no-undefined $(KDE_PLUGIN)


messages: rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/kstyle_baghira_config.pot

pixmaps.h: $(srcdir)/generatePixmaps.sh
	$(SHELL) $(srcdir)/generatePixmaps.sh $(top_srcdir)
CLEANFILES = pixmaps.h
kstyle_baghira_config.lo: pixmaps.h

