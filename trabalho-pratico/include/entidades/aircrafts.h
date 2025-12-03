#ifndef AIRCRAFTS_H
#define AIRCRAFTS_H

typedef struct aeronave {
    char *identifier;
    char *manufacturer;
    char *model;
    int year;
    int capacity;
    float range;
} Aeronave;

#endif