#include "point.h"
#include "clist.h"
#include "curve.h"
#include "stdio.h"
#include "math.h"

void initCurve(Curve *curve)
{
    double length = 0, area = 0;
    Node *node;
    Node *node_now;
    Point *loopPoint;
    Point *loopPointNext;
    Point *lowPoint;
    Point *highPoint;
    node = curve->list->head_node;
    if (curve->list != NULL && curve->list->size > 0)
    {
        lowPoint = curve->list->head_node->data;
        highPoint = curve->list->head_node->data;
        while (node != NULL)
        {
            node_now = node;
            node = node_GetNext(node);
            loopPoint = node_now->data;
            if (node != NULL)
            {
                loopPointNext = node->data;
                length += calcPointLength(loopPoint, loopPointNext);
                area += calcPointArea(loopPoint, loopPointNext);
                if (loopPointNext->y < lowPoint->y)
                    lowPoint = loopPointNext;
                if (loopPointNext->y > highPoint->y)
                    highPoint = loopPointNext;
            }
        }
    }
    curve->length = length;
    curve->area = area;
    curve->lowPoint = lowPoint;
    curve->highPoint = highPoint;
}

void mvCurve(Curve *curve, double shiftX, double shiftY)
{
    Node *node;
    Node *node_now;
    Point *loopPoint;
    Point *loopPointNext;
    node = curve->list->head_node;
    if (curve->list != NULL && curve->list->size > 0)
    {
        loopPoint = node->data;
        shiftPoint(loopPoint, shiftX, shiftY);
        while (node != NULL)
        {
            node_now = node;
            node = node_GetNext(node);
            loopPoint = node_now->data;
            if (node != NULL)
            {
                loopPointNext = node->data;
                shiftPoint(loopPointNext, shiftX, shiftY);
            }
        }
    }
}
