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

int ft_strlen(char *msg)
{
    int i = 0;

    while (msg[i])
        i++;
    return (i);
}

int free_all(FILE *file, char *draw, char *msg)
{
    fclose(file);
    if (draw)
        free(draw);
    if (msg)
        return (write(1, msg, ft_strlen(msg)));
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
    if (x < shape->x || shape->x + shape->width < x 
    || y < shape->y || shape->y + shape->height < y)
        return (0);
    if (x - shape->x < 1 || (shape->x + shape->width) - x < 1 
        || y - shape->y < 1 || (shape->y + shape->height) - y < 1)
        return 2;
    return (1);
}

int drawing(FILE *file, t_zone *zone, char *draw)
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
}

int main(int argc, char **argv)
{
    FILE *file;
    t_zone zone;
    int i;
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
    if (fscanf(file, "%d %d %c\n", &zone.width, &zone.height, &zone.bkgrnd) != 3)
    {
        free_all(file, NULL, "Error: Operation file corrupted\n");
        return (1);
    }
    if (!check_zone(&zone))
    {
        free_all(file, NULL, "Error: Operation file corrupted\n");
        return (1);
    }
    d = (char *)malloc(sizeof(char) * (zone.width * zone.height));
    if (!d)
        return (free_all(file, NULL, NULL));
    i = 0;
    while (i < zone.width * zone.height)
        d[i++] = zone.bkgrnd;
    if (!drawing(file, &zone, d))
        return (1);
    i = 0;
    while (i < zone.height)
    {
        write(1, d + (i * zone.width), zone.width);
        write(1, "\n", 1);
        i++;
    }
    free_all(file, d, NULL);
    return (0);
}