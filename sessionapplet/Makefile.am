INCLUDES= $(all_includes)
METASOURCES = AUTO

lib_LTLIBRARIES = usermanager_panelapplet.la

usermanager_panelapplet_la_SOURCES = usermanager.cpp dmctl.cpp
usermanager_panelapplet_la_LDFLAGS = -module -avoid-version $(all_libraries)
usermanager_panelapplet_la_LIBADD = $(LIB_KDEUI)

usermanager_DATA = usermanager.desktop
usermanagerdir = $(kde_datadir)/kicker/applets

messages: rc.cpp
	$(EXTRACTRC) `find . -name \*.ui -o -name \*.rc` > rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/usermanager.pot
