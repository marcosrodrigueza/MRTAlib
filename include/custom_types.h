#ifndef CUSTOM_TYPES_H
#define CUSTOM_TYPES_H

typedef struct
{
    float x;
    float y;
    float z;

} Point3D;

enum Urgency { urgent, important, not_important };
enum allocType { NAIVE_MARKET, CUSTOM_ALLOC };

#endif // CUSTOM_TYPES_H
