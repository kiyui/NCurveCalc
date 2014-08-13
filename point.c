#include "point.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

Point *mkPoint(double x, double y)
{
	Point *point = (Point *) malloc(sizeof(Point));
	point->x = x;
	point->y = y;
	return point;
}

void rmPoint(Point *point)
{
	if (point != NULL)
	{
		free(point);
	}
}

void shiftPoint(Point *point, double x, double y)
{
	point->x += x;
	point->y += y;
}

double calcPointLength(Point *point1, Point *point2)
{
    double length;
    length = sqrt(pow(point1->x - point2->x, 2) + pow(point1->y - point2->y, 2));
    return length;
}
