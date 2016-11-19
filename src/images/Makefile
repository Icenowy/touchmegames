
SUBDIRS=buttons cards games mainmenu submenu progressImages

install:
	if [ ! -e /usr/local/share/touchmegames/images/ ]; \
		then mkdir /usr/local/share/touchmegames/images/; \
	fi
	cp *.* /usr/local/share/touchmegames/images
	for i in ${SUBDIRS}; do \
		(cd $$i; ${MAKE} install;); \
	done  
