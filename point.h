#ifndef POINT_H
    #define POINT_H
typedef struct
{
	double x;
	double y;
}Point;

Point *mkPoint(double x, double y);
void rmPoint(Point *point);
void shiftPoint(Point *point, double x, double y);
double calcPointLength(Point *point1, Point *point2);
#endif
