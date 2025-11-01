#ifndef Q2_H
#define Q2_H

#include <glib.h>

typedef struct aeronave {
    char identifier[20];
    char manufacturer[50];
    char model[50];
    int year;
    int capacity;
    float range;
} Aeronave;

#endif