#include "clist.h"
#ifndef CURVE_H
    #define CURVE_H
/* @brief Curve structure, contains all information about a curve
 * */
typedef struct
{
    List *list;
    Point *lowPoint;
    Point *highPoint;
    double length;
    double area;
}Curve;

/* @brief Initializes curve memory
 * @param *curve Target Curve
 * */
void initCurve(Curve *curve);

/* @brief Shifts points in the curve
 * @param shiftX Value to shift X
 * @param shiftY Value to shift Y
 * */
void mvCurve(Curve *curve, double shiftX, double shiftY);
#endif
