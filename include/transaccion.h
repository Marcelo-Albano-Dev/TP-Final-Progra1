#ifndef TRANSACCION_H
#define TRANSACCION_H

#include "estructuras.h"

void menuTransacciones();
//MOSTRAR TRANSACCIONES
void listadoTransacciones(char archivoTrans[]);
void verTransaccionDetalle(char archivoTrans[], int idBuscado);
void mostrarTrans(Transaccion trans);

//REGISTRAR TRANSACCIONES Y CARRITO DE COMPRAS
int cargaVenta(char archivoTransacciones[]);
long generarIdTransaccion(char archivoTransaccion[]);
int registrarTransaccion(char archivoTransacciones[], Transaccion trans, int idJuego, int cantidadVendida);
#endif
