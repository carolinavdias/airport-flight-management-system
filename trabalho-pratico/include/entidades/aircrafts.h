#ifndef AIRCRAFTS_H
#define AIRCRAFTS_H

typedef struct aeronave Aeronave;

char *aircraft_get_identifier (Aeronave *a);
char *aircraft_get_manufacturer (Aeronave *a);
char *aircraft_get_model (Aeronave *a);
int aircraft_get_year(Aeronave *a);

void aircraft_set_id (Aeronave *a, char *id);
void aircraft_set_model (Aeronave *a, char *model_);
void aircraft_set_manuf (Aeronave *a, char *manuf);
void aircraft_set_year(Aeronave *a, int year);

int valida_aeronave(const char *linha);

Aeronave *criaAeronave(const char *linha);
void libertaAeronave(void *data);

#endif