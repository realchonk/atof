FUNCS = empty xstrtod custom custom2 dumb

# uncomment this line for more functions to benchmark
#FUNCS = empty xstrtof xstrtod xstrtold xatof custom custom_fast_isdigit dumb

# number of times to repeat data.in
N = 16384

COPT = -O2 -march=native
CFLAGS = -Wall -Wextra ${COPT}

all: ${FUNCS} custom3

run: ${FUNCS} custom3 $N.dat
	for f in ${FUNCS} custom3; do	\
		./bench $$f $N;		\
	done

clean:
	rm -f ${FUNCS} *.dat

${FUNCS}: test.c
	${CC} -o $@ test.c ${CFLAGS} -DFUNC=$@ -DFUNC_$@=1 -lm

custom3: custom3.c
	${CC} -o $@ custom3.c ${CFLAGS} -lm

$N.dat:
	rm -f $@
	for i in $$(seq $N); do cat data.in >> $@; done

