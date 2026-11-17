#ifndef PERSONA_H
#define PERSONA_H

#include <stdio.h>
#include "estructuras.h"

void menuPersonas();

// CRUD Base de Personas
int existePersonaPorDni(char dni[]);
void altaPersona();
void modificarPersonaPorDni(char dni[]);
void bajaLogicaPersona(char dni[]);
void listarPersonasResumen();
void mostrarPersonaDetallada(char dni[]);

// Funciones de Usuarios y Login
void inicializarAdminPorDefecto();
int existeNombreUsuario(char nombreUsuario[]);
void altaUsuario();
void listarUsuarios();
int iniciarSesion(Usuario *usuarioLogueado);

#endif
