FUNCS = empty custom2 dumb

# uncomment this line for more functions to benchmark
#FUNCS = empty xstrtof xstrtod xstrtold xatof custom custom_fast_isdigit dumb

# number of times to repeat data.in
N = 16384

FAST_POW10F = 1

COPT = -O2 -march=native
CFLAGS = -std=c99 -Wall -Wextra ${COPT}

all: ${FUNCS} custom3 dumb2

run: ${FUNCS} custom3 dumb2 $N.dat
	for f in ${FUNCS} custom3 dumb2; do	\
		./bench $$f $N;			\
	done

clean:
	rm -f ${FUNCS} custom3 mmap *.core

distclean: clean
	rm -f *.dat

${FUNCS}: test.c
	${CC} -o $@ test.c ${CFLAGS} -DFUNC=$@ -DFUNC_$@=1 -DFAST_POW10F=${FAST_POW10F} -lm

custom3: custom3.c
	${CC} -o $@ custom3.c ${CFLAGS} -lm

mmap: mmap.c
	${CC} -o $@ mmap.c ${CFLAGS} -lm

dumb2: dumb2.c
	${CC} -o $@ dumb2.c ${CFLAGS} -lm

$N.dat:
	rm -f $@
	for i in $$(seq $N); do cat data.in >> $@; done

