#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct	s_zone
{
	int		width;
	int		height;
	char	background;
}				t_zone;

typedef struct	s_shape
{
	char	type;
	float	x;
	float	y;
	float	radius;
	char	color;
}				t_shape;

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void	exit_program(FILE *file, char *str)
{
	fclose(file);
	if (str)
		free(str);
}

char	*get_zone(FILE *file, t_zone *zone)
{
	int		scan_ret;
	int		i;
	char	*drawing;

	scan_ret = fscanf(file, "%d %d %c\n", &zone->width, &zone->height, &zone->background);
	if (scan_ret != 3)
		return (0);
	if (zone->width <= 0 || zone->width > 300
			|| zone->height <= 0 || zone->height > 300)
		return (0);
	drawing = malloc(sizeof(char) * (zone->width * zone->height));
	if (!drawing)
		return (NULL);
	i = 0;
	while (i < (zone->width * zone->height))
	{
		drawing[i] = zone->background;
		i++;
	}
	return (drawing);
}

int	in_circle(float x, float y, t_shape *shape)
{
	float	distance;

	distance = sqrtf(((x - shape->x) * (x - shape->x)) + ((y - shape->y) * (y - shape->y)));
	if (distance <= shape->radius)
	{
		if ((shape->radius - distance) < 1.0)
			return (2);
		return (1);
	}
	return (0);
}

void	draw_shape(char *drawing, t_shape *shape, t_zone *zone)
{
	int	i;
	int	j;
	int	ret;

	i = 0;
	while (i < zone->height)
	{
		j = 0;
		while (j < zone->width)
		{
			ret = in_circle(j, i, shape);
			if (((shape->type == 'c') && ret == 2)
					|| ((shape->type == 'C') && ret))
				drawing[(i * zone->width) + j] = shape->color;
			j++;
		}
		i++;
	}
}

int	draw_shapes(FILE *file, t_zone *zone, char *drawing)
{
	t_shape	shape;
	int		scan_ret;

	while ((scan_ret = fscanf(file, "%c %f %f %f %c\n", &shape.type, &shape.x, &shape.y, &shape.radius, &shape.color))== 5)
	{
		if ((shape.radius > 0) && (shape.type == 'c' || shape.type == 'C'))
			draw_shape(drawing, &shape, zone);
		else
			return (0);
	}
	if (scan_ret != -1)
		return (0);
	return (1);
}

void	draw_painting(char *drawing, t_zone *zone)
{
	int	i;

	i = 0;
	while (i < zone->height)
	{
		write(1, drawing + (i * zone->width), zone->width);
		write(1, "\n", 1);
		i++;
	}
}

int	main(int argc, char *argv[])
{
	FILE	*file;
	char	*drawing;
	char	*error1;
	char	*error2;
	t_zone	zone;

	error1 = "Error: argument\n";
	error2 = "Error: Operation file corrupted\n";
	if (argc != 2)
	{
		write(1, error1, ft_strlen(error1));
		return (1);
	}
	file = fopen(argv[1], "r");
	if (!file)
	{
		write(1, error2, ft_strlen(error2));
		return (1);
	}
	drawing = get_zone(file, &zone);
	if (!drawing)
	{
		exit_program(file, NULL);
		write(1, error2, ft_strlen(error2));
		return (1);
	}
	if (draw_shapes(file, &zone, drawing) == 0)
	{
		exit_program(file, drawing);
		write(1, error2, ft_strlen(error2));
		return (1);
	}
	draw_painting(drawing, &zone);
	exit_program(file, drawing);
	return (0);
}
