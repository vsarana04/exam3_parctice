#ifndef MINI_PAINT
# define MINI_PAINT

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

typedef struct s_zone
{
	int width;
	int height;
	char bkgrnd;
} t_zone;

typedef struct s_shape
{
	char type;
	float x;
	float y;
	float radius;
	char color;
} t_shape;

#endif

