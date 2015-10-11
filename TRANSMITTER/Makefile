CC=gcc
# Hey!, I am comment number 2. I want to say that CFLAGS will be the
# options I'll pass to the compiler.
CFLAGS=-l bcm2835
#CFLAGS_WPI=-l wiringPi
DEPS=pattern.c

install:
	sudo cp -p energenie /usr/local/bin/
	sudo cp -p homeeasy /usr/local/bin/
	sudo cp -p drayton /usr/local/bin/
	sudo cp -p bgas /usr/local/bin/

all: 1on 1off 2on 2off 3on 3off 4on 4off masteron masteroff homeeasy toggle bgas drayton energenie light

homeeasy: homeeasy.c
	$(CC) -l rt -o $@ $< $(DEPS) $(CFLAGS)
bgas: bgas.c pattern.c
	$(CC) -l rt -o $@ $< $(DEPS) $(CFLAGS) 
drayton: drayton.c
	$(CC) -l rt -o $@ $< $(DEPS) $(CFLAGS)
energenie: energenie.c pattern.c
	$(CC) -l rt -o $@ $< $(DEPS) $(CFLAGS)
light: light.c pattern.c
	$(CC) -l rt -o $@ $< $(DEPS) $(CFLAGS)

clean:
	rm -rf homeeasy bgas drayton energenie light
