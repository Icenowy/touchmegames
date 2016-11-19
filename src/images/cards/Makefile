
SUBDIRS=backs default

install:
	if [ ! -e /usr/local/share/touchmegames/images/cards ]; \
		then mkdir /usr/local/share/touchmegames/images/cards; \
	fi
	cp *.* /usr/local/share/touchmegames/images/cards; \
	for i in ${SUBDIRS}; do \
		if [ ! -e /usr/local/share/touchmegames/images/cards/$$i ]; \
			then mkdir /usr/local/share/touchmegames/images/cards/$$i; \
		fi; \
		cp $$i/*.* /usr/local/share/touchmegames/images/cards/$$i; \
	done 
