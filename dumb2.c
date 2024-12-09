#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include "pow10f.h"

#define fgetc(file) getc_unlocked((file))

float ident (float f)
{
	return f;
}
static float (*volatile black_box)(float) = ident;

bool dumb2 (FILE *file, float *out)
{
	float f = 0.0f, x;
	int ival, sign, exp;
	char *s, buf[23];

	if (fread (buf, 1, sizeof (buf), file) != sizeof (buf))
		return false;

	s = buf;
	sign = *s++ == '-' ? -1 : 1;
	ival = *s++ - '0';

	// skip '.'
	++s;

	x = 0.1f;
	for (int i = 0; i < 14; ++i, x *= 0.1f)
		f += (*s++ - '0') * x;

	// skip 'e-'
	s += 2;

	exp = 0;
	for (int i = 0; i < 3; ++i)
		exp = exp * 10 + (*s++ - '0');

	*out = sign * (ival + f) * pow10f (-1, exp);
	return 1;
}

int main (int argc, char *argv[])
{
	float f;

	(void)argv;

	setvbuf (stdin, NULL, _IOFBF, 1 << 20);

	if (argc == 1) {
		while (dumb2 (stdin, &f))
			black_box (f);
	} else {
		while (dumb2 (stdin, &f))
			printf ("%f\n", black_box (f));
	}

	return 0;
}
