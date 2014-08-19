#ifndef POINT_H
    #define POINT_H
typedef struct
{
	double x;
	double y;
}Point;

/* @brief Allocates memory a Point structure
 * @param x Value of x
 * @param y Value of y
 * @return Memory of new Point
 * */
Point *mkPoint(double x, double y);

/* @brief Frees memory allocated to a Point structure
 * @param *point Target Point
 * */
void rmPoint(Point *point);

/* @brief Shifts a Point's coordinates
 * @param x X value to shift Point
 * @param y Y value to shift Point
 * */
void shiftPoint(Point *point, double x, double y);

/* @brief Calculates the distance between two Points
 * @param *point1 First target Point
 * @param *point2 Second target Point
 * @return Distance between *point1 & *point2
 * */
double calcPointLength(Point *point1, Point *point2);

double calcPointArea(Point *point1, Point *point);
#endif
