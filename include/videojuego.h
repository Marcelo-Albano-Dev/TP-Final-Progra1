#ifndef VIDEOJUEGO_H
#define VIDEOJUEGO_H

#include "estructuras.h"

// Estructura auxiliar para tu punto de promoción
typedef struct stDevolucion {
    int idDevolucion;
    int idTransaccionAnulada;
    int idJuego;
    int cantidadDevuelta;
    Fecha fechaDevolucion;
    char motivo[100];
} Devolucion;

// Menú de tu módulo
void menuVideojuegos();

// Puntos obligatorios (3e, 3f, 3g) + Alta
void cargarVideojuego();
void listarVideojuegosResumen();
void listarVideojuegosPorPlataforma();
void verDetalleVideojuego(int idJuego);

// Funciones que van a llamar Dani y Tomi desde sus archivos
int buscarVideojuegoPorId(int idJuego, Videojuego *juegoEncontrado);
int actualizarStockVideojuego(int idJuego, int cambioStock);
int obtenerUltimoIdVideojuego();

// Punto de promoción (Devoluciones)
void registrarDevolucion();
void listarDevoluciones();
int transaccionYaDevuelta(int idTransaccion);
int obtenerUltimoIdDevolucion();

#endif // VIDEOJUEGO_H// VIDEOJUEGO_H