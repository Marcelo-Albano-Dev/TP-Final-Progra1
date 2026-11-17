#ifndef TRANSACCION_H
#define TRANSACCION_H

#include "estructuras.h"
// Estructura auxiliar
typedef struct {
    int idJuego;
    int cantidad;
    float precioAdquisicion;
} ItemCarrito;


void menuTransacciones();

//MOSTRAR TRANSACCIONES
void listadoTransacciones(char archivoTrans[]);
void verTransaccionDetalle(char archivoTrans[], int idBuscado);
void mostrarTrans(Transaccion trans);

//REGISTRAR TRANSACCIONES Y CARRITO DE COMPRAS
long generarIdTransaccion(char archivoTransaccion[]);
int registrarTransaccion(char archivoTransacciones[], Transaccion trans, int idJuego, int cantidadVendida);
int buscarIndiceEnCarrito(ItemCarrito *carrito, int cantItems, int idBuscado);
ItemCarrito* redimensionCarrito(ItemCarrito *carrito, int *cantItems, ItemCarrito nuevoItem);
ItemCarrito* cargarItemsCarrito(int *cantItems);
int procesarCarritoCompras(char archivoTransacciones[]);
#endif
