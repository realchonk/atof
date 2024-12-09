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

bool custom3 (FILE *file, float *out)
{
	float f = 0.0f, x;
	int ch, sign = 1, ival, exp;

retry:
	do {
		ch = fgetc (file);
	} while (ch == ' ');

	switch (ch) {
	case '#':
		do {
			ch = fgetc (file);
		} while (ch != '\n');
		// fallthrough
	case '\n':
		goto retry;
	case '-':
		sign = -1;
		ch = fgetc (file);
		break;
	case EOF:
		return false;
	}

	for (ival = 0; isdigit (ch); ch = fgetc (file))
		ival = ival * 10 + (ch - '0');

	if (ch != '.')
		goto skip;
	
	for (x = 0.1f, ch = fgetc (file); isdigit (ch); x *= 0.1f, ch = fgetc (file))
		f += (ch - '0') * x;

skip:
	f = sign * (f + ival);

	if (ch != 'e' && ch != 'E')
		goto ret;

	sign = 1;
	ch = fgetc (file);
	if (ch == '-') {
		sign = -1;
		ch = fgetc (file);
	}

	for (exp = 0; isdigit (ch); ch = fgetc (file))
		exp = exp * 10 + (ch - '0');

	f *= pow10f (sign, exp);

ret:
	for (; ch != '\n'; ch = fgetc (file));
	*out = f;
	return true;
}

int main (int argc, char *argv[])
{
	float f;

	(void)argv;

	setvbuf (stdin, NULL, _IOFBF, 1 << 20);

	if (argc == 1) {
		while (custom3 (stdin, &f))
			black_box (f);
	} else {
		while (custom3 (stdin, &f))
			printf ("%f\n", black_box (f));
	}

	return 0;
}
