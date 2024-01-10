#include "micro_paint.h"

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

int check_shape(t_shape *shape)
{
	return (shape->width > 0 && shape->height > 0);
}

int in_shape(float x, float y, t_shape *shape)
{
	if (x < shape->x || shape->x + shape->width < x || y < shape->y || shape->y + shape->height < y)
		return (0);
	if (x - shape->x < 1 || (shape->x + shape->width) + x < 1 
		|| y - shape->y < 1 || (shape->y + shape->height) + y < 1)
		return (2);
	return (1);
}

void drawing(FILE *file, t_zone *zone, char *d)
{
	t_shape shape;
	int x, y, ret;

	while ((ret = fscanf(file, "%c %f %f %f %f %c\n", &shape.type, &shape.x, &shape.y, &shape.width, &shape.height, &shape.color)) == 6)
	{
		if ((shape.type != 'r' && shape.type != 'R') || !check_shape(&shape))
			break ;
		y = 0;
		while (y < zone->height)
		{
			x = 0;
			while (x < zone->width)
			{
				ret = in_shape((float)x, (float)y, &shape);
				if ((shape.type == 'r' && ret == 2) || (shape.type == 'R' && ret))
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
