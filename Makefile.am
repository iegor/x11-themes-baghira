
if HAVE_3_4
KICKERMENUDIR=kickermenu
SIDEBARDIR=sidebar
else
KICKERMENUDIR=kickermenu-3.3
SIDEBARDIR=
endif

SUBDIRS = bab colorscheme config deco icons sessionapplet starter style $(KICKERMENUDIR) $(SIDEBARDIR)

$(top_srcdir)/configure.in: configure.in.in $(top_srcdir)/subdirs
	cd $(top_srcdir) && $(MAKE) -f admin/Makefile.common configure.in ;

$(top_srcdir)/subdirs:
	cd $(top_srcdir) && $(MAKE) -f admin/Makefile.common subdirs

$(top_srcdir)/acinclude.m4: $(top_srcdir)/admin/acinclude.m4.in $(top_srcdir)/admin/libtool.m4.in
	@cd $(top_srcdir) && cat admin/acinclude.m4.in admin/libtool.m4.in > acinclude.m4

MAINTAINERCLEANFILES = configure.in acinclude.m4 configure.files 

package-messages:
	$(MAKE) -f admin/Makefile.common package-messages
	$(MAKE) -C po merge

EXTRA_DIST = admin COPYING configure.in.in

dist-hook:
	cd $(top_distdir) && perl admin/am_edit -padmin
	cd $(top_distdir) && $(MAKE) -f admin/Makefile.common subdirs
