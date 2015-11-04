CC=gcc
# Hey!, I am comment number 2. I want to say that CFLAGS will be the
# options I'll pass to the compiler.
CFLAGS=-l bcm2835
#CFLAGS_WPI=-l wiringPi
DEPS=pattern.c
BINDIR ?=/usr/local/bin/
CONFIGDIR ?=/etc
CONFIGURE ?=./configure
MAKE_INSTALL ?=sudo make install
AUTOMAKE ?= automake --add-missing
ACLOCAL ?= aclocal
MAKE ?=make


prep:
	@cd /home/pi
	sudo apt-get install automake
	cd ../bcm2835-1.46 && $(CONFIGURE)
	cd ../bcm2835-1.46 && $(MAKE)
	cd ../bcm2835-1.46 && $(MAKE_INSTALL)


install: prep all
	sudo cp -p energenie /usr/local/bin/
	sudo cp -p light /usr/local/bin/
	sudo cp -p drayton /usr/local/bin/
	sudo cp -p bgas /usr/local/bin/

all: bgas drayton energenie light

bgas: bgas.c pattern.c
	$(CC) -l rt -o $@ $< $(DEPS) $(CFLAGS) 
drayton: drayton.c
	$(CC) -l rt -o $@ $< $(DEPS) $(CFLAGS)
energenie: energenie.c pattern.c
	$(CC) -l rt -o $@ $< $(DEPS) $(CFLAGS)
light: light.c pattern.c
	$(CC) -l rt -o $@ $< $(DEPS) $(CFLAGS)

clean:
	rm -rf bgas drayton energenie light
