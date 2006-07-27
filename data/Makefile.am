AUTOMAKE_OPTIONS = foreign

data_DATA = example.conf

EXTRA_DIST = example.conf

install-data-local:
	@if test -f $(sysconfdir)/services.conf ; then \
		echo "$@ will not overwrite existing $(sysconfdir)/services.conf"; \
	else \
		$(mkinstalldirs) $(sysconfdir); \
		echo "$(INSTALL_DATA) example.conf $(sysconfdir)/services.conf"; \
		$(INSTALL_DATA) example.conf $(sysconfdir)/services.conf; \
	fi
	$(mkinstalldirs) $(localstatedir)

uninstall-local:
	@if test -f $(sysconfdir)/services.conf ; then \
		echo "rm -f $(sysconfdir)/services.conf"; \
		$(RM) -f $(sysconfdir)/services.conf; \
	fi
