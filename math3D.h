#ifndef MATH3D
#define MATH3D

#include <math.h>

typedef double* V3;

static inline void v3_add(V3 c, V3 a, V3 b)
{
	c[0] = a[0] + b[0];
	c[1] = a[1] + b[1];
	c[2] = a[2] + b[2];
}

static inline void v3_subtract(V3 c, V3 a, V3 b)
{
	c[0] = a[0] - b[0];
	c[1] = a[1] - b[1];
	c[2] = a[2] - b[2];
}


static inline void v3_scale(V3 c, V3 a, double b)
{
	c[0] = a[0] * b;
	c[1] = a[1] * b;
	c[2] = a[2] * b;
}

static inline double v3_dot(V3 a, V3 b)
{
	double c;
	c = a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
	return c;
}

static inline void v3_cross( V3 c, V3 a, V3 b)
{
	c[0] = a[1]*b[2] - a[2]*b[1];
	c[1] = a[2]*b[0] - a[0]*b[2];
	c[2] = a[0]*b[1] - a[1]*b[0];
}

V3 v3_assign(double a, double b, double c)
{
	V3 vector = malloc(sizeof(double) * 3);

	vector[0] = a;
	vector[1] = b;
	vector[2] = c;

	return vector;
}

V3 v3_unit(double a, double b, double c)
{
	V3 vector = malloc(sizeof(double) * 3);

	vector[0] = pow(a/(a*a+b*b+c*c),0.5);
	vector[1] = pow(b/(a*a+b*b+c*c),0.5);
	vector[2] = pow(c/(a*a+b*b+c*c),0.5);

	return vector;
}

#endif