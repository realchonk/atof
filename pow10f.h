
#if FAST_POW10F
# define NEXP 20

/*
static float expsp[NEXP] = {
	1e0f, 1e1f, 1e2f, 1e3f, 1e4f, 1e5f, 1e6f, 1e7f, 1e8f, 1e9f,
	1e10f, 1e11f, 1e12f, 1e13f, 1e14f, 1e15f, 1e16f, 1e17f, 1e18f, 1e19f,
};
static float expsn[NEXP] = {
	1e-0f, 1e-1f, 1e-2f, 1e-3f, 1e-4f, 1e-5f, 1e-6f, 1e-7f, 1e-8f, 1e-9f,
	1e-10f, 1e-11f, 1e-12f, 1e-13f, 1e-14f, 1e-15f, 1e-16f, 1e-17f, 1e-18f, 1e-19f,
};
*/
static float exps[2 * NEXP] = {
	1e-20f, 1e-19f, 1e-18f, 1e-17f, 1e-16f, 1e-15f, 1e-14f, 1e-13f, 1e-12f, 1e-11f,
	1e-10f, 1e-9f, 1e-8f, 1e-7f, 1e-6f, 1e-5f, 1e-4f, 1e-3f, 1e-2f, 1e-1f,
	1e0f, 1e1f, 1e2f, 1e3f, 1e4f, 1e5f, 1e6f, 1e7f, 1e8f, 1e9f,
	1e10f, 1e11f, 1e12f, 1e13f, 1e14f, 1e15f, 1e16f, 1e17f, 1e18f, 1e19f,
};
inline static float fast_pow10f (int sign, int exp)
{
	return exps[sign * exp + 20];
	/*
	if (exp < NEXP) {
		return sign > 0 ? expsp[exp] : expsn[exp];
	} else {
		return powf (10.0f, sign * exp);
	}
	*/
}

# define pow10f(sign, exp) (fast_pow10f ((sign), (exp)))
#else
# define pow10f(sign, exp) (powf (10.0f, (sign) * (exp)))
#endif
