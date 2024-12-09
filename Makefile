FUNCS = empty custom2 dumb

# uncomment this line for more functions to benchmark
#FUNCS = empty xstrtof xstrtod xstrtold xatof custom custom_fast_isdigit custom2 dumb

ALL = ${FUNCS} custom3 dumb2 custom4

# number of times to repeat data.in
N = 16384

FAST_POW10F = 1

COPT = -g -O2 -march=native
CFLAGS = -std=c99 -Wall -Wextra -D_GNU_SOURCE=1 -D_BSD_SOURCE=1 ${COPT}
CPPFLAGS = -DFAST_POW10F=${FAST_POW10F}

all: ${ALL}

run: ${ALL} $N.dat
	for f in ${ALL}; do	\
		./bench $$f $N;	\
	done

clean:
	rm -f ${ALL} *.core

distclean: clean
	rm -f *.dat

${FUNCS}: test.c
	${CC} -o $@ test.c ${CFLAGS} -DFUNC=$@ -DFUNC_$@=1 ${CPPFLAGS} -lm

custom3: custom3.c
	${CC} -o $@ custom3.c ${CFLAGS} ${CPPFLAGS} -lm

mmap: mmap.c
	${CC} -o $@ mmap.c ${CFLAGS} ${CPPFLAGS} -lm

dumb2: dumb2.c
	${CC} -o $@ dumb2.c ${CFLAGS} ${CPPFLAGS} -lm

custom4: custom4.c
	${CC} -o $@ custom4.c ${CFLAGS} ${CPPFLAGS} -lm

$N.dat:
	rm -f $@
	for i in $$(seq $N); do cat data.in >> $@; done

