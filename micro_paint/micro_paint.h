#ifndef MICRO_PAINT
# define MICRO_PAINT

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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
	float width;
	float height;
	char color;
} t_shape;

#endif
