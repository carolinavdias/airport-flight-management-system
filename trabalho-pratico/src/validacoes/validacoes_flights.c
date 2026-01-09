#define _XOPEN_SOURCE 700

#include "validacoes/validacoes_flights.h"
#include "validacoes/validacoes.h"
#include "utils/utils.h"
#include <string.h>

/* ============================================
 * VOO -> VALIDAÇÕES SINTÁTICAS
 * ============================================ */


/**
 * Função valida_estado de um voo
 */

static int valida_Estado(const char *s) {
    if (!s || strlen(s) == 0) return 0;
    return (strcmp(s, "On Time") == 0 || strcmp(s, "Delayed") == 0 || strcmp(s, "Cancelled") == 0);
}

/* ============================================
 * VOO -> VALIDAÇÃO LÓGICA
 * ============================================ */

static int valida_VOO (Voo *voo, GestorAircrafts *gestor_aeronaves) {

    //DESTINATION != ORIGIN
    if (strcmp(voo_get_code_origin(voo), voo_get_code_destination(voo)) == 0) {
        return 0;
    }

    //AIRCRAFT EXISTE
    if (gestor_aeronaves && voo_get_id_aircraft(voo)) {
        if (!gestor_aircrafts_existe(gestor_aeronaves, voo_get_id_aircraft(voo))) {
            return 0;
        }
    }

    //ARRIVAL >= DEPARTURE
    if (voo_get_arrival(voo) < voo_get_departure(voo)) {
        return 0;
    }

    //se CANCELLED, actual_departure E actual_arrival devem ser -2 (N/A)
    if (voo_get_status(voo) == ESTADO_CANCELLED) {
        if (voo_get_actual_departure(voo) != -2 ||
            voo_get_actual_arrival(voo) != -2) {
            return 0;
        }
    }

    //se NÃO CANCELLED: actual_arrival >= actual_departure
    if (voo_get_status(voo) != ESTADO_CANCELLED) {
        if (voo_get_actual_departure(voo) != -2 &&
            voo_get_actual_arrival(voo) != -2) {
            if (voo_get_actual_arrival(voo) < voo_get_actual_departure(voo)) {
                return 0;
            }
        }
    }

    // se DELAYED, actual >= scheduled
    if (voo_get_status(voo) == ESTADO_DELAYED) {
        // actual_departure >= departure
        if (voo_get_actual_departure(voo) != -2 &&
            voo_get_actual_departure(voo) < voo_get_departure(voo)) {
            return 0;
        }
        //actual_arrival >= arrival
        if (voo_get_actual_arrival(voo) != -2 &&
            voo_get_actual_arrival(voo) < voo_get_arrival(voo)) {
            return 0;
        }
    }

    return 1; //válido
}


Voo *validacoes_campos_flights(char **campos, GestorAircrafts *AC, StringPool *pool) {
    Voo *v = criaVoo();

    if (valida_id_voo(campos[0])     && // id_flight
        valida_DataH(campos[1])      && //departure
        valida_DataH(campos[3])      && // arrival
        valida_Estado(campos[6])     && // status
        valida_codigoIATA(campos[7]) && //codigoIATA origem
        valida_codigoIATA(campos[8]) )  //codigoIATA destino
    {


        voo_set_flight_id (v,campos[0],pool);
        voo_set_dataH(v,campos[1],1);
        voo_set_dataH(v,campos[3],3);
        voo_set_status(v,campos[6]);
        voo_set_code(v,campos[7],'o',pool);
        voo_set_code(v,campos[8],'d',pool);
        voo_set_id_aircraft(v,campos[9],pool);
        voo_set_airline(v,campos[10],pool);

         //actual departure/arrival
         if (voo_get_status(v) == ESTADO_CANCELLED) {
              if (strcmp(campos[2],"N/A") != 0 || strcmp(campos[4],"N/A") != 0) return NULL;
              else {
                  voo_set_dataH(v,"",2);
                  voo_set_dataH(v,"",4);
              }
         } else if (valida_DataH(campos[2]) && valida_DataH(campos[4])) {
                  voo_set_dataH(v,campos[2],2);
                  voo_set_dataH(v,campos[4],4);
                } else {
		  libertaVoo(v);
		  return NULL;
	        }

	if (!valida_VOO(v, AC)) {
	      libertaVoo(v);
	      return NULL;
	}

 	return v;
    }
    else {
	libertaVoo(v);
	return NULL;
    }

}
