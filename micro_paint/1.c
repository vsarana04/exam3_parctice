#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct s_zone
{
    int width;
    int height;
    char bkground;
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

int error(char *msg)
{
    write(1, msg, 35);  // Додаємо \n в кінці рядка
    return (1);
}

int free_all(FILE *file, char *draw, char *msg)
{
    fclose(file);
    if (draw)
        free(draw);
    if (msg)
        return (error(msg));
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
    if (x - shape->x < 1.00000000 && (shape->x - shape->width) + x < 1.00000000 
        && y - shape->y < 1.00000000 && (shape->y - shape->height) + y < 1.00000000)
        return (2);
    return (1);
}

int drawing(FILE *file, t_zone *zone, char *d)
{
    t_shape shape;
    int x, y, ret;

    while ((ret = fscanf(file, "%c %f %f %f %f %c\n", &shape.type, &shape.x, &shape.y, &shape.width, &shape.height, &shape.color)) == 6)
    {
        if ((shape.type != 'r' && shape.type != 'R') || !check_shape(&shape))
            break;

        y = 0;
        while (y < zone->height)
        {
            x = 0;
            while (x < zone->width)
            {
                ret = in_shape((float)x, (float)y, &shape);
                if ((shape.type == 'r' && ret == 2) || (shape.type == 'R' && ret == 1))
                    d[(y * zone->width) + x] = shape.color;
                x++;
            }
            y++;
        }
    }

    if (ret != -1)
    {
        error("Error: Operation file corrupted\n");
        return (0);
    }
    return (1);
}

int main(int argc, char **argv)
{
    FILE *file;
    t_zone zone;
    int i;
    char *draw;

    if (argc != 2)
        return (error("Error: argument\n"));  // Замінено на правильний текст помилки
    if (!(file = fopen(argv[1], "r")))
        return (error("Error: Operation file corrupted\n"));
    if (fscanf(file, "%d %d %c\n", &zone.width, &zone.height, &zone.bkground) != 3)
        return (free_all(file, NULL, "Error: Operation file corrupted\n"));
    if (!check_zone(&zone))
        return (free_all(file, NULL, "Error: Operation file corrupted\n"));
    draw = (char *)malloc(sizeof(char) * (zone.width * zone.height));
    if (!draw)
        return (free_all(file, NULL, NULL));
    i = 0;
    while (i < zone.width * zone.height)
        draw[i++] = zone.bkground;
    if (!drawing(file, &zone, draw))
        return (1);
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
