#ifndef TRANSACCION_H
#define TRANSACCION_H

#include "estructuras.h"

void menuTransacciones();
void listadoTransacciones(char archivoTrans[]);
void verTransaccionDetalle(char archivoTrans[], int idBuscado);
void mostrarTrans(Transaccion trans);
#endif
