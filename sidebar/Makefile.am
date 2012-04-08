INCLUDES= $(all_includes)
METASOURCES = AUTO

kde_module_LTLIBRARIES=konqsidebar_baghirasidebar.la
include_HEADERS = baghirasidebar.h linkview.h listboxlink.h dndlistbox.h baghirasidebariface.h
noinst_HEADERS = baghiralinkdrag.h baghirasidebar.h linkview.h listboxlink.h dndlistbox.h baghirasidebariface.h

konqsidebar_baghirasidebar_la_SOURCES = baghiralinkdrag.cpp baghirasidebar.cpp linkview.cpp listboxlink.cpp dndlistbox.cpp linkconfig.ui baghirasidebariface.skel
konqsidebar_baghirasidebar_la_LDFLAGS = -module $(KDE_PLUGIN) $(all_libraries) -lkonqsidebarplugin
konqsidebar_baghirasidebar_la_LIBADD = $(LIB_KPARTS) $(LIB_KFILE)

baghirasidebar_entry_DATA = baghirasidebar.desktop
baghirasidebar_entrydir = $(kde_datadir)/konqsidebartng/entries
baghirasidebar_add_DATA = baghirasidebar_add.desktop
baghirasidebar_adddir = $(kde_datadir)/konqsidebartng/add
poof_DATA = ../imagebase/poof.png
poofdir = $(kde_datadir)/baghira
