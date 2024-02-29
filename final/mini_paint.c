#include "paint.h"

void error(FILE *file ,char *draw)
{
    fclose(file);
    if (draw)
        free(draw);
}

char    *get_zone(FILE *file, t_zone *zone)
{
    int ret, i;
    char *draw;

    ret = fscanf(file, "%d %d %c\n", &zone->width, &zone->height, &zone->bkgrnd);
    if (ret != 3)
        return (0);
    if (zone->width <= 0 || zone->width > 300 || zone->height <= 0 || zone->height > 300)
        return (0);
    draw = malloc(sizeof(char) * (zone->width * zone->height));
    if (!draw)
        return (NULL);
    i = 0;
    while (i < (zone->width * zone->height))
    {
        draw[i] = zone->bkgrnd;
        i++;
    }
    return (draw);
}

int in_circle(float x, float y, t_shape *shape)
{
    float dist;

    dist = sqrtf(((x - shape->x) * (x - shape->x)) + ((y - shape->y) * (y - shape->y)));
    if (dist <= shape->radius)
    {
        if (shape->radius - dist < 1.0)
            return (2);
        return (1);
    }
    return (0);
}

void draw_shape(char *draw, t_shape *shape, t_zone *zone)
{
    int x, y, ret;

    y = 0;
    while (y < zone->height)
    {
        x = 0;
        while (x < zone->width)
        {
            ret = in_circle(x, y, shape);
            if (((shape->type == 'c') && ret == 2) || ((shape->type == 'C') && ret))
                draw[(y * zone->width) + x] = shape->color;
            x++;
        }
        y++;
    }
}

int draw_shapes(FILE *file, t_zone *zone, char *draw)
{
    t_shape shape;
    int ret;

    while ((ret = fscanf(file, "%c %f %f %f %c\n", &shape.type, &shape.x, &shape.y, &shape.radius, &shape.color)) == 5)
    {
        if (shape.radius > 0.0 && (shape.type == 'c' || shape.type == 'C'))
            draw_shape(draw, &shape, zone);
        else
            return (0);
    }
    if (ret != -1)
        return (0);
    return (1);
}

void    draw(char *draw, t_zone *zone)
{
    int i;
    
    i = 0;
    while (i < zone->height)
    {
        write(1, draw + (i * zone->width), zone->width);
        write(1, "\n", 1);
        i++;
    }
}

int main(int argc, char *argv[])
{
    FILE *file;
    t_zone zone;
    char *d;

    if (argc != 2)
    {
        write(1, "Error: argument\n", 17);
        return (1);
    }
    file = fopen(argv[1], "r");
    if (!file)
    {
        write(1, "Error: Operation file corrupted 1\n", 33);
        return (1);
    }
    d = get_zone(file, &zone);
    if (!d)
    {
        error(file, NULL);
        write(1, "Error: Operation file corrupted 2\n", 33);
        return (1);
    }
    if (draw_shapes(file, &zone, d) == 0)
    {
        error(file, d);
        write(1, "Error: Operation file corrupted 3\n", 33);
        return (1);
    }
    draw(d, &zone);
    error(file, d);
    return (0);
}