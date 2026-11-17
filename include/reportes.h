#ifndef REPORTES_H
#define REPORTES_H

#include <stdio.h>
#include "estructuras.h"

#define ARCH_REPORTES_PRECIO "data/reportes_precio.dat"
#define STOCK_CRITICO 5

typedef struct {
    int idReportes;
    int idJuego;
    float precioAdquisicionViejo;
    float precioAdquisicionNuevo;
    Fecha fechaModificacion;
} ReportesPrecio;

// Menu principal del modulo
void menuReportes();

// Reportes principales
void mostrarReporteMensual();
void ventaMayorGanancia();
void mostrarStockCritico();
void listarReportesPrecio();
void reporteGananciasPorFecha();

// Funciones auxiliares de ordenamiento, recursividad y fechas
Videojuego cargarStockCritico(int cantidad);
void intercambiarVideojuegos(Videojuego vj1, Videojuego vj2);
int particionStock(Videojuego juegos[], int inicio, int fin);
void quickSortStock(Videojuego juegos[], int inicio, int fin);
float analisisGanancias(FILE *buffer, Fecha desde, Fecha hasta);
int consultarCambioPrecio(int idJuego, int mes, int anio);
int compararFechas(Fecha f1, Fecha f2);

#endif // REPORTES_H
