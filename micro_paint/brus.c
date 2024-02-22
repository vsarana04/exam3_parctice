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
    float width;
    float height;
    float x;
    float y;
    char c;
} t_shape;

void free_all(FILE *file, char *draw)
{
    fclose(file);
    if (draw)
        free(draw);
}

/*int check_zone(t_zone *zone)
{
    return (zone->width > 0 && zone->width <= 300 && zone->height > 0 && zone->height <= 300);
}

int check_shape(t_shape *shape)
{
    return (shape->width > 0 && shape->height > 0);
}*/

char    *get_zone(FILE *file, t_zone *zone)
{
    int i;
    int ret;
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
    if (i < (zone->width * zone->height))
    {
        draw[i] = zone->bkgrnd;
        i++;
    }
    return (draw);
}
int in_shape(float x, float y, t_shape *shape)
{
    if (((x < shape->x || (shape->x + shape->width < x))
        || (y < shape->y)) || (shape->y + shape->height < y))
        return (0);
    if (((x - shape->x < 1.0) || ((shape->x + shape->width) - x < 1.0))
            || ((y - shape->y < 1.0 || ((shape->y + shape->height) - y < 1.0))))
        return (2);
    return (1);
}

void draw_shape(char *drawing, t_shape *shape, t_zone *zone)
{
    int x, y, ret;

    y = 0;
    while (y < zone->height)
    {
        x = 0;
        while (x < zone->width)
        {
            ret = in_shape(x, y, shape);
            if ((shape->type == 'r' && ret == 2) || (shape->type == 'R' && ret))
                drawing[(y * zone->width) + x] = shape->c;
            x++;
        }
        y++;

    }
}

int draw_shapes(FILE *file, t_zone *zone, char *drawing)
{
    t_shape shape;
    int ret;

    while ((ret = fscanf(file, "%c %f %f %f %f %c\n", &shape.type, &shape.x, &shape.y, &shape.width, &shape.height, &shape.c)) == 6)
    {
        if (shape.width > 0 && shape.height > 0 && (shape.type == 'r' || shape.type == 'R'))
            draw_shape(drawing, &shape, zone);
        else
            return (0);
    }
    if (ret != -1)
        return (0);
    return (1);
}
 void draw(char *drawing, t_zone *zone)
 {
    int i;
    
    i = 0;
    while (i < zone->height)
    {
        write(1, drawing + (i * zone->width), zone->width);
        write(1, "\n", 1);
        i++;
    }
 }

/*int drawing(FILE *file, t_zone *zone, char *draw)
{
    t_shape shape;
    int x, y, ret;

    while ((ret = fscanf(file, "%c %f %f %f %f %c\n", &shape.type, &shape.x, &shape.y, &shape.width, &shape.height, &shape.c)) == 6)
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
                    draw[(y * zone->width) + x] = shape.c;
                x++;
            }
            y++;
        }
    }
    if (ret != -1)
    {
        write(1, "Error: Operation file corrupted\n", 33);  
        return (0);
    }
    return (1);
}*/

int main(int argc, char **argv)
{
    FILE *file;
    t_zone zone;
    char *d;

    if (argc != 2)
    {
        write(1, "Error: argument\n", 17);
        return (1);
    }
    if (!(file = fopen(argv[1], "r")))
    {
        write(1, "Error: Operation file corrupted\n", 33);
        return (1);
    }
    d = get_zone(file, &zone);
    if (!d)
    {
        free_all(file, NULL);
        write(1, "Error: Operation file corrupted\n", 33);
        return (1);
    }
    if (draw_shapes(file, &zone, d) == 0)
    {
        free_all(file, NULL);
        write(1, "Error: Operation file corrupted\n", 33);
        return (1);
    }
    draw(d, &zone);
    free_all(file, d);
    return (0);
}