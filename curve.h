#include "clist.h"
#ifndef CURVE_H
    #define CURVE_H
typedef struct
{
    List *list;
    Point *lowPoint;
    Point *highPoint;
    double length;
    double area;
}Curve;

void initCurve(Curve *curve);
void mvCurve(Curve *curve, double shiftX, double shiftY);
double areaUnderCurve(Curve *curve);
#endif
