## Makefile.am for bab

# set the include path for X, qt and KDE
INCLUDES = $(all_includes)

# these are the headers for your project
noinst_HEADERS = bab.h bab_iface.h styleconfdialog.h

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO

messages: rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/kdedcoptest.pot

# this is the program that gets installed.  it's name is used for all
# of the other Makefile.am variables
bin_PROGRAMS = bab

# the application source, library search path, and link libraries
bab_SOURCES = main.cpp styleconfdialog.cpp bab_iface.skel
bab_LDFLAGS = $(KDE_RPATH) $(all_libraries)
bab_LDADD = $(LIB_KDEUI)
