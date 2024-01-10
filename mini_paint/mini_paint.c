#include "mini_paint.h"

int ft_strlen(char *msg)
{
	int i = 0;
	while (msg)
		i++;
	return (i);
}

int free_all(FILE *file, char *draw, char *msg)
{
	fclose(file);
	if (draw)
		free(draw);
	if (msg)
		return (write(1, msg, ft_strlen(msg)), 1);
	return (1);
}

int check_zone(t_zone *zone)
{
	return (zone->width > 0 && zone->width <= 300 && zone->height > 0 && zone->height <= 300);
}

int in_circle(float x, float y, t_shape *shape)
{
	float dist;

	dist = sqrtf(powf(x - shape->x, 2.0) + powf(y - shape->y, 2.0));
	if (dist <= shape->radius)
	{
		if (shape->type == 'C')
			return (1);
		return (shape->radius - dist < 1.0);
	}
	return (0);
}

void drawing(FILE *file, t_zone *zone, char *d)
{
	t_shape shape;
	int x, y, ret;

	while ((ret = fscanf(file, "%c %f %f %f %c\n", &shape.type, &shape.x, &shape.y, &shape.radius, &shape.color)) == 5)
	{
		if ((shape.type != 'c' && shape.type != 'C') || shape.radius <= 0.0)
			break ;
		y = 0;
		while (y < zone->height)
		{
			x = 0;
			while (x < zone->width)
			{
				if (in_circle((float)x, (float)y, &shape))
					d[(y * zone->width) + x] = shape.color;
				x++;
			}
			y++;
		}
	}
	if (ret != -1)
		write(1, "Error: Operation file corrupted\n", 40);

}

int main(int argc, char **argv)
{
	FILE *file;
	t_zone zone;
	int i;
	char *draw;

	if (argc != 2)
		return (write(1, "Error: Operation file corrupted\n", 40), 1);
	if (!(file = fopen(argv[1], "r")))
		return (free_all(file, NULL, "Error: Operation file corrupted\n"), 1);
	if (fscanf(file, "%d %d %c\n", &zone.width, &zone.height, &zone.bkgrnd) != 3)
		return (free_all(file, NULL, "Error: Operation file corrupted\n"));
	if (!check_zone(&zone))
		return (free_all(file, NULL, NULL));
	draw = (char *)malloc(sizeof(char) * (zone.width * zone.height));
	if (!draw)
		return (free_all(file, NULL, NULL));
	i = 0;
	while (i < zone.width * zone.height)
		draw[i++] = zone.bkgrnd;
	drawing(file, &zone, draw);
	i = 0;
	while (i < zone.height)
	{
		write(1, draw + (i * zone.width), zone.width);
		write(1, "\n", 1);
		i++;
	}
	free_all(file, draw, NULL);
	return (0);

}

