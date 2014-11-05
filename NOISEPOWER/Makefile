CC=gcc
# Hey!, I am comment number 2. I want to say that CFLAGS will be the
# options I'll pass to the compiler.
CFLAGS=-l bcm2835
DEPS=pattern.c

all: 1on 1off 2on 2off 3on 3off 4on 4off masteron masteroff homeeasy toggle bgas drayton energenie

toggle: toggle.c
	$(CC) -l rt -o $@ $< $(CFLAGS)
homeeasy: homeeasy.c
	$(CC) -l rt -o $@ $< $(DEPS) $(CFLAGS)
1on: 1on.c pattern.c
	$(CC) -l rt -o $@ $< $(DEPS) $(CFLAGS)
1off: 1off.c pattern.c
	$(CC) -l rt -o $@ $< $(DEPS) $(CFLAGS)
2on: 2on.c pattern.c
	$(CC) -l rt -o $@ $< $(DEPS) $(CFLAGS)
2off: 2off.c pattern.c
	$(CC) -l rt -o $@ $< $(DEPS) $(CFLAGS)
3on: 3on.c pattern.c
	$(CC) -l rt -o $@ $< $(DEPS) $(CFLAGS)
3off: 3off.c pattern.c
	$(CC) -l rt -o $@ $< $(DEPS) $(CFLAGS)
4on: 4on.c pattern.c
	$(CC) -l rt -o $@ $< $(DEPS) $(CFLAGS)
4off: 4off.c pattern.c
	$(CC) -l rt -o $@ $< $(DEPS) $(CFLAGS)
masteron: master_on.c pattern.c
	$(CC) -l rt -o $@ $< $(DEPS) $(CFLAGS)
masteroff: master_off.c pattern.c
	$(CC) -l rt -o $@ $< $(DEPS) $(CFLAGS)
bgas: bgas.c
	$(CC) -l rt -o $@ $< $(CFLAGS)
drayton: drayton.c
	$(CC) -l rt -o $@ $< $(CFLAGS)
light: light_socket.c
	$(CC) -l rt -o $@ $< $(CFLAGS)
energenie: energenie.c
	$(CC) -l rt -o $@ $< $(CFLAGS)

clean:
	rm -rf 1on 1off 2on 2off 3on 3off 4on 4off masteron masteroff homeeasy toggle bgas drayton light
