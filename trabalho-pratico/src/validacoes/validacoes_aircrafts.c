#include "validacoes/validacoes_aircrafts.h"
#include <string.h>

/* ============================================
 * AERONAVES -> VALIDAÇÃO SINTÁTICA
 * ============================================ */

/**
 * O ano deve ter exatamente 4 dígitos numéricos e ser menor ou igual a 2025.
 * A função converte manualmente a string para inteiro para garantir
 * validação sintática rigorosa.
 */

int valida_year(const char *s) {

    int year = 0;
    if (!s || strlen(s) != 4) return 0;
    for (int i = 0; i < 4; i++) {
        if (s[i] < '0' || s[i] > '9') return 0;
	else year = year * 10 + s[i]-'0';
    }
    return year <= 2025;
}

/**
 * Esta função assume que o array `campos` contém:
 *  - campos[0] → ID da aeronave
 *  - campos[1] → fabricante
 *  - campos[2] → modelo
 *  - campos[3] → ano de fabrico
 *
 * Apenas o ano é validado sintaticamente neste módulo.
 * Se o ano for válido, a aeronave é criada e preenchida.
 */

Aeronave *validacoes_campos_aircrafts(char **campos, GHashTable *lista_strings) {
    Aeronave *ac = criaAeronave();

    if (valida_year(campos[3])) //ano fabrico aeronave
    {

        // Preenchimento dos campos
        aircraft_set_id(ac,campos[0]);
	    aircraft_set_manuf(ac,campos[1],lista_strings);
	    aircraft_set_model(ac,campos[2]);
        aircraft_set_year(ac,campos[3]);
        return ac;

    } else {
	libertaAeronave(ac);
	return NULL;
    }
}

