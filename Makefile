FUNCS = empty xstrtod custom custom_fast_isdigit dumb
#FUNCS = empty xstrtof xstrtod xstrtold xatof custom custom_fast_isdigit dumb

COPT = -O2 -march=native
CFLAGS = -Wall -Wextra ${COPT}
N = 65536

all: ${FUNCS}

run: ${FUNCS} $N.dat
	for f in ${FUNCS}; do	\
		./bench $$f $N;	\
	done

clean:
	rm -f ${FUNCS} *.dat

${FUNCS}: test.c
	${CC} -o $@ test.c ${CFLAGS} -DFUNC=$@ -lm

$N.dat:
	rm -f $@
	for i in $$(seq $N); do cat data.in >> $@; done

