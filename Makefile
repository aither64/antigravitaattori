export PREFIX = /usr/local
export DATADIR = $(PREFIX)/share/antigrav

all:
	$(MAKE) -C src

clean:
	$(MAKE) -C src clean

install: all
	mkdir -p "$(DATADIR)"
	mv src/antigrav "$(PREFIX)"/bin/
	cp data/* "$(DATADIR)"/
	chown -R root:root "$(PREFIX)"/bin/antigrav "$(DATADIR)"/

uninstall:
	rm -rf "$(DATADIR)"
	rm -f "$(PREFIX)"/bin/antigrav

.PHONY: all clean install uninstall tinyxml
