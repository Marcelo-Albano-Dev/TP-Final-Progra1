#ifndef PERSONA_H
#define PERSONA_H

#include <stdio.h>
#include "estructuras.h"

void menuPersonas();

// CRUD Base de Personas
int existePersonaPorDni(char dni[]);
void altaPersona();
void modificarPersonaPorDni(char dni[]);
void listarPersonasResumen();
void mostrarPersonaDetallada(char dni[]);

#endif
