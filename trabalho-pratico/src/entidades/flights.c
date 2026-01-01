#include "entidades/flights.h"
#include <glib.h>

typedef struct voo {
    char *flight_id; //voo_id
    long long departure; //partida_prevista
    long long actual_departure; //partida_efetiva
    long long arrival; //chegada prevista
    long long actual_arrival; //chegada efetiva
    //char *gate; //porta_embarque
    Estado status; //++ Estado
    char *code_origin; //codigo IATA origem
    char *code_destination; //codigo IATA destino
    char *id_aircraft; //id_aeronave
    char *airline; //companhia aerea
    //char *tracking_url;
} Voo;

//GETTERS
char *voo_get_flight_id (const Voo *v) {
    return g_strdup(v->flight_id); 
}

const char *voo_get_code_origin (const Voo *v) {
    return v->code_origin;  // const char* não precisa g_strdup
}

long long voo_get_departure (const Voo *v) {
    return v->departure;
}

long long voo_get_actual_departure (const Voo *v) {
    return v->actual_departure;
}

Estado voo_get_status (const Voo *v) {
    return v->status;
}

const char *voo_get_id_aircraft (const Voo *v) {
    return v->id_aircraft;  // const char* não precisa g_strdup
}

const char *voo_get_code_destination (const Voo *v) {
    return v->code_destination;  // const char* não precisa g_strdup
}

long long voo_get_arrival (const Voo *v) {
    return v->arrival;
}

long long voo_get_actual_arrival (const Voo *v) {
    return v->actual_arrival;
}

//SETTERS
/*
void voo_set_gate (Voo *v, char *campo_gate) {
   g_free(v->gate);
   v->gate = g_strdup(campo_gate);
}

void voo_set_tracking_url (Voo *v, char *url) {
   g_free(v->tracking_url);
   v->tracking_url = g_strdup(url);
}
*/

void voo_set_code (Voo *v, char *code, char versao) {
   switch (versao) {
	case 'o' : g_free(v->code_origin);
		   v->code_origin = g_strdup(code);
		   break;
 	case 'd' : g_free(v->code_destination);
		   v->code_destination = g_strdup(code);
		   break;
   }
}

void voo_set_flight_id (Voo *v, char *id) {
   g_free(v->flight_id);
   v->flight_id = g_strdup(id);
}

void voo_set_id_aircraft (Voo *v, char *id_airc) {
   g_free(v->id_aircraft);
   v->id_aircraft = g_strdup(id_airc);
}

void voo_set_airline (Voo *v, char *airl) {
   g_free(v->airline);
   v->airline = g_strdup(airl);
}

void voo_set_status (Voo *v, char *status) {
   switch (status[0]) {
	case 'O' : v->status = ESTADO_ON_TIME;
	      break;
	case 'D' : v->status = ESTADO_DELAYED;
	      break;
	case 'C' : v->status = ESTADO_CANCELLED;
	      break;
   }
}

long long converte_dataH (const char *s) {

    // extrair números
    int ano  = (s[0]-'0')*1000 + (s[1]-'0')*100 + (s[2]-'0')*10 + (s[3]-'0');
    int mes  = (s[5]-'0')*10   + (s[6]-'0');
    int dia  = (s[8]-'0')*10   + (s[9]-'0');
    int hora = (s[11]-'0')*10  + (s[12]-'0');
    int min  = (s[14]-'0')*10  + (s[15]-'0');

    // construir valor inteiro ordenável
    return (long long)ano * 100000000 +
           (long long)mes *   1000000 +
           (long long)dia *     10000 +
           (long long)hora *      100 +
           (long long)min ;
}

void voo_set_dataH (Voo *v, const char *s, int campo) {

    if (s[0] == '\0' && campo == 2) { v->actual_departure = (long long)-2; return; }
    if (s[0] == '\0' && campo == 4) { v->actual_arrival = (long long)-2; return; }

    switch (campo) {
        case 1 : v->departure = converte_dataH(s);
                 break;
        case 2 : v->actual_departure = converte_dataH(s);
                 break;
        case 3 : v->arrival = converte_dataH(s);
                 break;
        case 4 : v->actual_arrival = converte_dataH(s);
                 break;

    }
}

//CRIA/LIBERTA
Voo *criaVoo () {
    Voo *v = calloc (1, sizeof *v);
    return v;
}

void libertaVoo(void *data) {
    Voo *v = data;
    if (!v) return;
    g_free(v->flight_id);
    //g_free(v->gate);
    g_free(v->code_origin);
    g_free(v->code_destination);
    g_free(v->id_aircraft);
    g_free(v->airline);
    //g_free(v->tracking_url);
    g_free(v);
}