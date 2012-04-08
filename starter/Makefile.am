INCLUDES= $(all_includes)
METASOURCES = AUTO
KDE_ICON = AUTO

lib_LTLIBRARIES = libbaghirastarter.la

libbaghirastarter_la_SOURCES = baghiralinkdrag.cpp menu.cpp starter.cpp starterconfig.ui starterhelp.ui config.ui help.ui linkconfig.ui starteriface.skel
libbaghirastarter_la_LDFLAGS = -module -avoid-version $(all_libraries)
libbaghirastarter_la_LIBADD = -lXtst $(LIB_KDEUI) 

starter_DATA = starter.desktop
starterdir = $(kde_datadir)/kicker/applets
poof_DATA = ../imagebase/poof.png
poofdir = $(kde_datadir)/baghira

messages: rc.cpp
	$(EXTRACTRC) *.ui >> rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/starter.pot
