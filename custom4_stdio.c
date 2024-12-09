#include <sys/stat.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <err.h>
#include "pow10f.h"

float ident (float f)
{
	return f;
}
static float (*volatile black_box)(float) = ident;

#define abort() while (1)

bool custom4 (const char **sp, float *out)
{
	float f = 0.0f, x;
	int ival = 0, sign, exp;
	const char *s = *sp;

	switch (*s) {
	case '\0':
		return false;
	case '-':
		sign = -1;
		break;
	case ' ':
		sign = 1;
		break;
	default:
		abort ();
	}
	++s;

	for (; isdigit (*s); ++s)
		ival = ival * 10 + (*s - '0');

	if (*s != '.')
		goto skip;
	++s;

	for (x = 0.1f; isdigit (*s); ++s, x *= 0.1f)
		f += (*s - '0') * x;

skip:
	f = sign * (f + ival);

	if (*s != 'e' && *s != 'E')
		goto ret;
	++s;

	sign = 1;
	if (*s == '-') {
		sign = -1;
		++s;
	}

	for (exp = 0; isdigit (*s); ++s)
		exp = exp * 10 + (*s - '0');

	f *= pow10f (sign, exp);

ret:
	if (*s != '\n')
		abort ();
	*sp = s + 1;
	*out = f;
	return true;
}

int main (int argc, char *argv[])
{
	FILE *file = stdin;
	struct stat st;
	char *s, *buf;
	float f;

	(void)argv;

	if (fstat (fileno (file), &st) != 0)
		err (1, "fstat()");

	if (st.st_size == 0)
		errx (1, "st.st_size == 0");

	buf = malloc (st.st_size + 1);
	if (fread (buf, 1, st.st_size, file) != (size_t)st.st_size)
		err (1, "fread()");
	buf[st.st_size] = '\0';


	if (argc == 1) {
		for (s = buf; custom4 ((const char **)&s, &f);)
			black_box (f);
	} else {
		for (s = buf; custom4 ((const char **)&s, &f);)
			printf ("%f\n", f);
	}

	free (buf);
	return 0;
}

