#ifndef VIDEOJUEGO_H
#define VIDEOJUEGO_H

#include "estructuras.h"

#define ARCHIVO_DEVOLUCIONES "data/devoluciones.dat"
#define MARGEN_GANANCIA 1.30f

// Estructura auxiliar para devoluciones
typedef struct stDevolucion {
    int idDevolucion;
    int idTransaccionAnulada;
    int idJuego;
    int cantidadDevuelta;
    Fecha fechaDevolucion;
    char motivo[100];
} Devolucion;

// Menu principal del modulo
void menuVideojuegos();

// Alta, modificacion, baja logica y consultas
void cargarVideojuego();
void modificarVideojuego(int idJuego);
void bajaLogicaVideojuego(int idJuego);
void listarVideojuegosResumen();
void listarVideojuegosPorPlataforma();
void verDetalleVideojuego(int idJuego);

// Funciones compartidas para Transacciones y Reportes
int buscarVideojuegoPorId(int idJuego, Videojuego *JuegoEncontrado);
int actualizarStockVideojuego(int idJuego, int cambioStock);
int obtenerUltimoIdVideojuego();

// Devoluciones
void registrarDevolucion();
void listarDevoluciones();
int transaccionYaDevuelta(int idTransaccion);
int obtenerUltimoIdDevolucion();

#endif // VIDEOJUEGO_H
